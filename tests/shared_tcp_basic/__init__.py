#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014-2015 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function

import subprocess
import os
import shlex
try:
    from test_utils import *
except ImportError:
    import sys
    parent_dir = os.path.join(os.path.dirname(__file__), "..")
    sys.path.append(parent_dir)
    from test_utils import *

NANOCAT_BIN = "../../venv/bin/nanocat"
MASTER_URL = "tcp://127.0.0.1:12345"
LUA_CODE = 'print("hi from master")'

def run_test(spell, lib):
    assert spell
    assert lib

    # Get test name
    name = os.path.basename(os.path.dirname(__file__))

    # Execute master
    log("Executing master")
    master_spell = [normalized_path(__file__, NANOCAT_BIN),
        "--req",
        "--bind", MASTER_URL,
        "--data", LUA_CODE,
    ]
    log("spell = " + " ".join(master_spell))
    master_proc = subprocess.Popen(master_spell)

    # Execute target spell
    log("Executing target spell")
    log("spell = " + spell)


    env = os.environ
    if sys.platform == "darwin":
        env["DYLD_INSERT_LIBRARIES"] = lib
        spell = shlex.split(spell)
    elif sys.platform == "win32":
        raise AssertionError("Can't handle Windows at the momment")
    else:
        env["LD_PRELOAD"] = lib
        spell = shlex.split(spell)

    target_proc = subprocess.Popen(spell,
        env=env,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    master_proc.wait()
    log("Terminating target spell")
    target_proc.terminate()

    log("Searching patterns")
    stdout_data, stderr_data = target_proc.communicate()
    hit_master = False
    print("")
    for line in stderr_data.splitlines():
        log(line, name + " - stderr")
    print("")
    for line in stdout_data.splitlines():
        if line == "hi from master":
            hit_master = True
        log(line, name + " - stdout")

    return 0 if hit_master and master_proc.returncode == 0 else 1
    return master_proc.returncode
