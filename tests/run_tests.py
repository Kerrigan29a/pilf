#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014-2015 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function

import pkgutil
import os

def main():
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
    exit(main())
