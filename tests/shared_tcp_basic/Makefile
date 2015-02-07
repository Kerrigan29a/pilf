# Copyright (c) 2014-2015 Javier Escalada Gómez
# All rights reserved.

MAKEFILE_FILE = $(abspath $(firstword $(MAKEFILE_LIST)))
MAKEFILE_PATH = $(dir $(MAKEFILE_FILE))
CURRENT_DIR = $(patsubst %/,%,$(MAKEFILE_PATH))
PILF_BASE_DIR = $(abspath $(CURRENT_DIR)/../..)/

DEBUG = 1

include ../../Makefile.vars
include ../../deps/Makefile.vars
include ../../src/Makefile.vars
include ../Makefile.vars



.PHONY: all minion clean distclean

all: minion

minion: test_shared.c $(TEST_DEPS)
	$(CC) $(CPPFLAGS) $(TEST_CFLAGS) $(TEST_LDFLAGS) $^ -o $@

clean:
	-rm -f minion
	-rm -rf *.dSYM
	-rm -f *.pyc

distclean: clean