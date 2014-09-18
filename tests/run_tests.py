#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function

# import os
# import os.path

# def get_python_files(root_path='.'):
#     assert(root_path)
#     return (root + os.sep + f for root, dirs, files in os.walk(root_path)
#         for f in files if f.endswith(".py") and root != ".")

# def run_tests():
#     for f in get_python_files():
#         name, _ = f[1:].split(".")
#         name = name.replace(os.sep, ".")
#         print(name)
#         module = __import__(name)


import pkgutil
import os

def run_tests():
    modules = pkgutil.iter_modules(path=["."])
    this_file, _ = os.path.basename(__file__).split(".")
    for loader, mod_name, ispkg in modules:
        if mod_name == this_file:
            continue
        module = __import__(mod_name)
        try:
            run_test = module.run_test
            print("\n\n" + "#" * 80)
            print("#   Test: " + mod_name)
            print("#" * 80)
            result = run_test()
            if result != 0:
                return 1
        except AttributeError:
            continue

if __name__ == "__main__":
    exit(run_tests())