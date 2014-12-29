#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

from __future__ import print_function

import subprocess
import time
try:
    from test_utils import *
except ImportError:
    import sys
    sys.path.append("..")
    from test_utils import *

NANOCAT_BIN = "../../venv/bin/nanocat"
MASTER_URL = "tcp://127.0.0.1:12345"
LUA_CODE = r"""local ffi = require 'ffi'

function printf(...)
 io.write(string.format(...))
end

local addrs = {address}

local hidden_func = ffi.cast('void (*)(void)', addrs)
hidden_func()
"""

ADDRESS_FILE = "address.txt"

def run_test():
    # Exec minion
    log("Executing minion")
    spell = [normalized_path(__file__, "./minion"), MASTER_URL]
    log("spell = " + " ".join(spell))
    minion_proc = subprocess.Popen(spell,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Get address of hidden_func
    address = 0
    log("Searching for hidden_func address")
    while True:
        try:
            with open(ADDRESS_FILE) as f:
                address = f.read().strip()
                log("Found hidden_func address at " + address)
                break
        except IOError:
            log("Can't find file " + ADDRESS_FILE)
            time.sleep(1)

    # Exec fake master with code
    log("Executing master")
    spell = [normalized_path(__file__, NANOCAT_BIN),
        "--req",
        "--bind", MASTER_URL,
        "--data", LUA_CODE.format(address=address),
    ]
    log("spell = " + " ".join(spell))
    master_proc = subprocess.Popen(spell)
    master_proc.wait()

    # Get lines from minion
    log("Terminating minion")
    minion_proc.terminate()
    log("Searching pattern")
    stdout_data, stderr_data = minion_proc.communicate()
    hit_hidden_func = False
    print("")
    for line in stderr_data.splitlines():
        log(line, "minion - stderr")
    print("")
    for line in stdout_data.splitlines():
        if line == "hi from hidden func":
            hit_hidden_func = True
        log(line, "minion - stdout")

    return 0 if hit_hidden_func and master_proc.returncode == 0 else 1

if __name__ == "__main__":
    exit(run_test())
