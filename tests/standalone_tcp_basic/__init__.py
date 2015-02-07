#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014-2015 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function

import subprocess
try:
    from test_utils import *
except ImportError:
    import sys
    import os.path
    parent_dir = os.path.join(os.path.dirname(__file__), "..")
    sys.path.append(parent_dir)
    from test_utils import *

NANOCAT_BIN = "../../venv/bin/nanocat"
MASTER_URL = "tcp://127.0.0.1:12345"
LUA_CODE = 'print("hi from master")'

def run_test(exe=None, cwd=None):

    # Get test name
    name = os.path.basename(os.path.dirname(__file__))

    exe = exe or normalized_path(__file__, name + "_minion")

    # Execute master
    log("Executing master:")
    spell = [normalized_path(__file__, NANOCAT_BIN),
        "--req",
        "--bind", MASTER_URL,
        "--data", LUA_CODE,
    ]
    log("    spell = " + " ".join(spell))
    master_proc = subprocess.Popen(spell)

    # Execute minion
    log("Executing minion:")
    spell = [exe, MASTER_URL]
    log("    spell = " + " ".join(spell))
    log("    cwd = " + str(cwd))
    minion_proc = subprocess.Popen(spell,
        cwd=cwd,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    master_proc.wait()
    log("Terminating minion")
    minion_proc.terminate()

    log("Searching patterns")
    stdout_data, stderr_data = minion_proc.communicate()
    hit_master = False
    hit_config_file = False
    print("")
    for line in stderr_data.splitlines():
        log(line, "tested - stderr")
    print("")
    for line in stdout_data.splitlines():
        if line == "hi from master":
            hit_master = True
        if line == "hi from config file":
            hit_config_file = True
        log(line, "tested - stdout")

    return 0 if hit_master and hit_config_file and master_proc.returncode == 0 else 1
    return master_proc.returncode
