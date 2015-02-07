#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014-2015 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function


import sys
import os.path
current_dir = os.path.dirname(__file__)
sys.path.append(current_dir)
from __init__ import *
try:
    from test_utils import *
except ImportError:
    parent_dir = os.path.join(os.path.dirname(__file__), "..")
    sys.path.append(parent_dir)
    from test_utils import *

if __name__ == "__main__":
    args = parse_test_standalone_args(current_dir)
    exit(run_test(**vars(args)))
