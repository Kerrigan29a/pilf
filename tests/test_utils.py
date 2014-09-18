#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function
import os.path

def log(txt, preffix = "     tester    "):
    print("[" + preffix + "] " + txt)

def normalized_path(current_file, target_file):
    final_path = os.path.dirname(current_file)
    return os.path.join(final_path, target_file)