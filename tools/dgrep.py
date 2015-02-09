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
import platform
import subprocess
import textwrap
import codecs

__VERSION__ = "2.1"



def debug(*objs):
    pass
    # print("\x1b[34m(D)\x1b[0m", *objs, file=sys.stderr)



class PreprocessorError(Exception):
    pass



class Preprocessor(object):

    def __init__(self, progname):
        assert(progname)

        self._progname = progname
        self.platform = self.available_platforms[0]["name"]

    def available_platforms():
        doc = "Available platforms"
        def fget(self):
            _os = platform.system()
            if _os == "Darwin":
                return [
                    {"name":"clang", "executable": "/usr/bin/clang", "spell": ["-E"]},
                    {"name":"gcc", "executable": "/usr/local/bin/gcc", "spell": ["-E"]},
                ]
            elif _os == "Linux":
                return [
                    {"name":"gcc", "executable": "/usr/bin/gcc", "spell": ["-E"]},
                    {"name":"clang", "executable": "/usr/bin/clang", "spell": ["-E"]},
                ]
            elif _os == "Windows":
                # TODO: Implement for Windows

                # [
                #     {"name":"cl", "executable": None, "spell": ["-E"]},
                #     {"name":"gcc", "executable": "/usr/bin/gcc", "spell": ["-E"]},
                #     {"name":"clang", "executable": "/usr/bin/clang", "spell": ["-E"]},
                # ]
                raise NotImplementedError("Not implemented for Windows")
            else:
                raise AssertionError("Unknown platform")
        return locals()
    available_platforms = property(**available_platforms())


    def platform():
        doc = "The selected platform"
        def fget(self):
            return self._platform
        def fset(self, value):
            for p in self.available_platforms:
                if p["name"] == value:
                    self._platform = value
                    self._executable = p["executable"]
                    self._spell = p["spell"]
                    return
            raise ValueError("Unknown platform")
        return locals()
    platform = property(**platform())

    def executable():
        doc = "The binary to execute"
        def fget(self):
            return self._executable
        def fset(self, value):
            if os.path.isfile(value):
                self._executable = value
            else:
                raise ValueError("Not a file: " + value)
        return locals()
    executable = property(**executable())

    def spell():
        doc = "The combination of arguments"
        def fget(self):
            return self._spell
        def fset(self, value):
            self._spell = value
        return locals()
    spell = property(**spell())

    def run(self, filename):
        full_spell = ([self.executable] + self.spell
            + ["-D" ,"__" + self._progname.upper() + "__"] + [filename])
        debug("full_spell -> " + unicode(full_spell))
        p = subprocess.Popen(full_spell, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = p.communicate()
        p.wait()
        if stderr:
            raise PreprocessorError(stderr)
        return stdout, stderr



class Parser(object):

    def __init__(self, preprocessor, progname, export_key, encoding):
        assert(preprocessor)
        assert(progname)
        assert(export_key)
        assert(encoding)

        self._preprocessor = preprocessor
        self._encoding = encoding

        self._patterns = []

        # Common pattern parts
        osp = r"[\\\s]+"
        zsp = r"[\\\s]*"
        pragma = "#" + zsp + "pragma"

        # Export pattern
        if preprocessor.platform == "cl":
            begin = "^" + zsp + "__pragma" + zsp + "\(" + zsp + progname + osp
        else:
            begin = "^" + zsp + pragma + osp + progname + osp
        arg = "export"  + osp + "(?P<export_key>" + export_key + ")" + osp + "(?P<export_line>" + "\d+" + ")"
        if preprocessor.platform == "cl":
            arg += "\)"
            end = ""
        else:
            end = zsp + "$"
        pattern = begin + arg + end
        self._patterns.append(pattern)
        debug("export pattern -> " + pattern)

        # Verbatim block pattern
        cmd = "verbatim_block"
        begin = "^" + zsp + pragma + osp + progname + osp + cmd + zsp
        arg = "^" + "(?P<" + cmd + ">.*?)" + zsp
        end = "^" + zsp + pragma + osp + progname + osp + cmd + "_end" + zsp + "$"
        pattern = begin + arg + end
        self._patterns.append(pattern)
        debug("verbatim_block pattern -> " + pattern)

        # Verbatim pattern
        cmd = "verbatim"
        begin = "^" + zsp + pragma + osp + progname + osp + cmd + zsp
        arg = '"(?P<' + cmd + '>.+?)"' + zsp
        end = "$"
        pattern = begin + arg + end
        self._patterns.append(pattern)
        debug("verbatim pattern -> " + pattern)

        # Compose and compile global pattern
        tmp_pattern = ")|(?:".join(self._patterns)
        global_pattern = "".join(["(?:", tmp_pattern, ")"])
        debug("global pattern -> " + global_pattern)
        self._global_pattern = re.compile(global_pattern, re.DOTALL | re.MULTILINE)

        # Declaration pattern
        begin = "^" + zsp
        arg = "(?P<declaration>" + export_key + osp + "[^;]+?)" + zsp
        end = "{"
        pattern = begin + arg + end
        self._declaration_pattern = re.compile(pattern, re.DOTALL | re.MULTILINE)
        debug("declaration pattern -> " + pattern)

    def _extract_declataration(self, txt):
        assert(txt)

        m = self._declaration_pattern.search(txt)
        return m.groupdict()["declaration"].strip()


    def _parse(self, filename):
        assert(filename)

        stdout, stderr = self._preprocessor.run(filename)
        debug("stdout ->\n" + stdout)
        debug("stderr ->\n" + stderr)

        for hit in (m.groupdict() for m in self._global_pattern.finditer(stdout)):

            if hit["export_key"] is not None and hit["export_line"] is not None:
                debug("EXPORT AT "+ hit["export_line"])
                lineno = int(hit["export_line"])
                declaration = self._extract_declataration("".join(self._file_lines[lineno - 1:])) + ";"
                debug(declaration)
                yield declaration
            elif hit["verbatim_block"] is not None:
                debug("VERBATIM")
                debug(hit["verbatim_block"])
                debug("END VERBATIM")
                yield hit["verbatim_block"]
            elif hit["verbatim"] is not None:
                debug("VERBATIM")
                debug(hit["verbatim"].decode('string_escape'))
                debug("END VERBATIM")
                yield hit["verbatim"].decode('string_escape')
            else:
                raise AssertionError("Unknown pragma command")

    def _load_file(self, filename):
        assert(filename)

        with codecs.open(filename, "r", self._encoding) as f:
            self._file_lines = f.readlines()

    def run(self, filename):
        assert(filename)

        self._load_file(filename)
        return (part for part in self._parse(filename))



class Composer(object):

    def __init__(self, parser, progname, export_key, ifilenames, ofilename=None,
            include_guard=True):
        assert(parser)
        assert(progname)
        assert(export_key)
        assert(ifilenames)

        self._parser = parser
        self._ifilenames = ifilenames
        self._ofilename = ofilename
        self._include_guard = include_guard

        datetime = time.strftime("%d/%m/%Y %H:%M:%S", time.gmtime())

        spell = " ".join(['"' + a + '"' if " " in a or "\t" in a else a for a in sys.argv])
        spell = " \\\n *     ".join(textwrap.wrap(spell, 80 - 7))
        self._comment = '''/*
 * {datetime}
 * {progname} version: {version}
 *
 * DO NOT EDIT!!!
 * This file was created automatically by:
 *
 *     {spell}
 */'''.format(
    spell=spell,
    datetime=datetime,
    progname=progname,
    version=__VERSION__)

        self._header = '''
#ifdef __cplusplus
extern "C" {{
#endif

#ifndef {pattern}
#define {pattern} extern
#endif
'''.format(pattern=export_key)

        self._footer = '''
#ifdef __cplusplus
}
#endif'''

        if self._ofilename:
            self._commonprefix = os.path.commonprefix(ifilenames + [ofilename])

        if self._include_guard:
            m = hashlib.sha1()
            m.update(datetime)
            m.update(unicode(ifilenames))
            if ofilename:
                m.update(ofilename)
            guard = "__" + m.hexdigest().upper() + "__"

            self._beguin_guard = '''
#ifndef {guard}
#define {guard}'''.format(guard=guard)

            self._end_guard = '''
#endif /* {guard} */
'''.format(guard=guard)

    def run(self):

        yield self._comment
        if self._include_guard:
            yield self._beguin_guard
        yield self._header

        for filename in self._ifilenames:
            short_filename = filename[len(self._commonprefix):] if self._ofilename else filename
            yield "/* " + short_filename + " */"
            for part in self._parser.run(filename):
                yield part

        yield self._footer

        if self._include_guard:
            yield self._end_guard



if __name__ == "__main__":

    import argparse

    def parse_script_args(default_platform, available_platforms, preprocessor_executable, preprocessor_spell):
        assert(default_platform)
        assert(available_platforms)
        assert(preprocessor_executable)
        assert(preprocessor_spell)

        parser = argparse.ArgumentParser(description='Declarations extractor')

        parser.add_argument('files', type=unicode, metavar='FILE', nargs='+',help='Source code files')
        parser.add_argument('-o', '--output', action='store', type=unicode, metavar='FILE', help='Output file')
        parser.add_argument('-p', '--pattern', action='store', type=unicode,metavar='TXT', default="public", help='Use this pattern (default: %(default)s)')
        parser.add_argument("-P", "--platform", action="store", type=unicode, metavar='NAME', default=default_platform, choices=available_platforms, help="Select one: %(choices)s (default: %(default)s)")
        parser.add_argument("-e", "--executable", metavar="BINARY", default=preprocessor_executable, help="Use this executable (default: %(default)s)")
        parser.add_argument("-s", "--spell", metavar="SPELL", action='append', default=preprocessor_spell, help="Use this spell (default: %(default)s)")
        parser.add_argument('-E', '--encoding', action='store', type=unicode, metavar='NAME', default="utf-8", help='Use this encoding  (default: %(default)s)')
        parser.add_argument("--dry", action='store_true', help="Run dry")

        return parser.parse_args()

    def parse_executable_args(args):
        return [token for part in args for token in part.split()]

    # Configure preprocessor
    progname = os.path.splitext(os.path.basename(__file__))[0]
    preprocessor = Preprocessor(progname)
    available_platforms = [p["name"] for p in preprocessor.available_platforms]
    script_args = parse_script_args(preprocessor.platform, available_platforms, preprocessor.executable, preprocessor.spell)
    preprocessor.platform = script_args.platform
    preprocessor.spell = parse_executable_args(script_args.spell)


    # Configure parser
    parser = Parser(preprocessor, progname, script_args.pattern, script_args.encoding)

    # Configure composer
    composer = Composer(parser, progname, script_args.pattern,
        script_args.files, script_args.output, include_guard=True)

    if script_args.dry:
        print("selected platform -> " + unicode(preprocessor.platform))
        print("selected executable -> " + unicode(preprocessor.executable))
        print("selected spell -> " + unicode(preprocessor.spell))
        print("applied pattern to preprocessed code -> ")
        for pattern in parser._patterns:
            print("  " + unicode(pattern))
        print("applied pattern to code -> ")
        print("  " + unicode(parser._declaration_pattern.pattern))
        exit(0)

    try:
        if script_args.output:
            with open(script_args.output, "w") as f:
                for line in composer.run():
                    f.write(line + "\n")
        else:
            for line in composer.run():
                print(line)
    except PreprocessorError as e:
        print("#" * 80, file=sys.stderr)
        print("# Preprocessor Error", file=sys.stderr)
        print("###")
        print(e.message, file=sys.stderr)
        exit(1)
