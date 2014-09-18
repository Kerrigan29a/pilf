#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function

import subprocess
try:
    from test_utils import *
except ImportError:
    import sys
    sys.path.append("..")
    from test_utils import *

NANOCAT_BIN = "../../venv/bin/nanocat"
MASTER_URL = "tcp://127.0.0.1:12345"
LUA_CODE = 'print("hi from master")'

def run_test():

    # Execute master
    log("Executing master")
    master_proc = subprocess.Popen([normalized_path(__file__, NANOCAT_BIN),
        "--req",
        "--bind", MASTER_URL,
        "--data", LUA_CODE,
    ])

    # Execute minion
    log("Executing minion")
    minion_proc = subprocess.Popen([normalized_path(__file__, "./minion"), MASTER_URL],
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
        log(line, "minion - stderr")
    print("")
    for line in stdout_data.splitlines():
        if line == "hi from master":
            hit_master = True
        if line == "hi from config file":
            hit_config_file = True
        log(line, "minion - stdout")

    return 0 if hit_master and hit_config_file and master_proc.returncode == 0 else 1
    return master_proc.returncode

if __name__ == "__main__":
    exit(run_test())