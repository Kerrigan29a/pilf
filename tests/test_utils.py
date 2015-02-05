#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function
import os.path
import argparse

def log(txt, preffix = "     tester    "):
    print("[" + preffix + "] " + txt)

def normalized_path(current_file, target_file):
    final_path = os.path.dirname(current_file)
    return os.path.join(final_path, target_file)

def parse_test_args(current_dir):
    parser = argparse.ArgumentParser(description='Test arguments')
    parser.add_argument('-e', '--exe',
        action='store',
        default=os.path.join(current_dir, "minion"),
        help='Minion exe location (default: %(default)s)')
    parser.add_argument('-d', '--cwd',
        action='store',
        default=current_dir,
        help='Working directory (default: %(default)s)')
    return parser.parse_args()