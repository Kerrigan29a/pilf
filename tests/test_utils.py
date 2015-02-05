#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function
import os.path
import argparse
import sys

def log(txt, preffix = "     tester    "):
    print("[" + preffix + "] " + txt)

def normalized_path(current_file, target_file):
    final_path = os.path.dirname(current_file)
    return os.path.join(final_path, target_file)

def parse_test_standalone_args(current_dir):
    parser = argparse.ArgumentParser(description='Test standalone arguments')
    
    parser.add_argument('-e', '--exe',
        action='store',
        default=os.path.join(current_dir, "minion"),
        help='Minion exe location (default: %(default)s)')
    
    parser.add_argument('-d', '--cwd',
        action='store',
        default=current_dir,
        help='Working directory (default: %(default)s)')
    
    return parser.parse_args()

def parse_test_shared_args(current_dir):
    parser = argparse.ArgumentParser(description='Test shared arguments')
    
    parser.add_argument('-s', '--spell',
        action='store',
        default="/bin/sleep 10",
        help='Target command spell (default: %(default)s)')

    if sys.platform == "darwin":
        lib_name = "libminion.dylib"
    elif sys.platform == "win32":
        lib_name = "libminion.dll"
    else:
        lib_name = "libminion.so"
    parser.add_argument('-l', '--lib',
        action='store',
        default=os.path.join(current_dir, lib_name),
        help='Shared lib location (default: %(default)s)')
    
    return parser.parse_args()