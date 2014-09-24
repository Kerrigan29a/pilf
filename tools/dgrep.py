#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Original idea from rubberhose project developed by:
# Julian Assange <proff@iq.org>
# Ralf-P. Weinmann <ralph@iq.org>

from __future__ import unicode_literals
from __future__ import print_function

import os
import sys
import re
import time
import hashlib

__VERSION__ = "0.3"



def filter_lines(pattern, filename):
    with open(filename) as f:
        return (g for m in pattern.finditer(f.read()) for g in m.groups() if g)



def parse(pattern, ifilenames, ofilename=None, include_guard=True):

    datetime = time.strftime("%d/%m/%Y %H:%M:%S", time.gmtime())

    comment = '''/*
 * {datetime}
 * {progname} version: {version}
 *
 * DO NOT EDIT!!!
 * This file was created automatically by:
 *
 * {spell}
 */'''.format(
    spell=" ".join(sys.argv),
    datetime=datetime,
    progname=os.path.splitext(os.path.basename(__file__))[0],
    version=__VERSION__)

    header = '''
#ifdef __cplusplus
extern "C" {{
#endif

#ifndef {pattern}
#define {pattern} extern
#endif
'''.format(pattern=pattern)

    footer = '''
#ifdef __cplusplus
}
#endif'''

    if ofilename:
        commonprefix = os.path.commonprefix(ifilenames + [ofilename])

    if include_guard:
        m = hashlib.sha1()
        m.update(datetime)
        m.update(str(ifilenames))
        if ofilename:
            m.update(ofilename)
        guard = "__" + m.hexdigest().upper() + "__"

        beguin_guard = '''
#ifndef {guard}
#define {guard}'''.format(guard=guard)

        end_guard = '''
#endif /* {guard} */
'''.format(guard=guard)

    # Compose pattern
    progname = os.path.splitext(os.path.basename(__file__))[0]
    patterns = [
        "(^\s*" + pattern + "[^;]+?){", 
        "^\s*#\s*pragma\s+" + progname
            + "\s+start_verbatim_block\s+(.*?)\s+^\s*#\s*pragma\s+"
            + progname + "\s+stop_verbatim_block"
    ]

    global_pattern = "(?:" + ")|(?:".join(patterns) + ")"
    compiled_pattern = re.compile(global_pattern, re.DOTALL | re.MULTILINE)

    result = [comment]

    if include_guard:
        result.append(beguin_guard)

    result.append(header)

    for filename in ifilenames:

        hits = list(filter_lines(compiled_pattern, filename))


        if hits:
            filename = filename[len(commonprefix):] if ofilename else filename
            result.append("/* " + filename + " */")        
        
        for l in hits:
            l = l.strip()
            if l.startswith(pattern):
                l += ";"
            result.append(l)

    result.append(footer)

    if guard:
        result.append(end_guard)

    return os.linesep.join(result)

if __name__ == "__main__":
    
    import argparse

    ############################################################################
    # Configure arguments
    ###

    parser = argparse.ArgumentParser(description='Extract declarations of methods')
    parser.add_argument('files', type=unicode, metavar='FILE', nargs='+',
        help='Source code files')
    parser.add_argument('-o', '--output', action='store', type=unicode, metavar='FILE',
        help='Output file')
    parser.add_argument('-p', '--pattern', action='store', type=unicode,
        metavar='TXT', default="PUBLIC",
        help='Use this pattern (default: %(default)s)')
    args = parser.parse_args()

    try:        
        txt = parse(args.pattern, args.files, args.output)
        if args.output:
            with open(args.output, "w") as fout:
                fout.write(txt)
        else:
            print(txt)
        exit(0)
    except IOError as e:
        print("Unable to open: " + e.message, file=sys.stderr)
        exit(1)