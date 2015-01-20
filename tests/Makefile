# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

MAKEFILE_FILE = $(abspath $(firstword $(MAKEFILE_LIST)))
MAKEFILE_PATH = $(dir $(MAKEFILE_FILE))
CURRENT_DIR = $(patsubst %/,%,$(MAKEFILE_PATH))
PILF_BASE_DIR = $(abspath $(CURRENT_DIR)/..)/

DEBUG = 1

include ../Makefile.vars
include ../src/Makefile.vars
include Makefile.vars

TESTER = python $(TESTS_DIR)/run_tests.py



################################################################################
# Main targets
###

.PHONY: all build_complex_tests clean distclean

all: build_complex_tests
	$(TESTER)


clean:
	-rm -rf $(SIMPLE_TESTS_TARGETS)
	-rm -rf *.dSYM
	-rm -f *.xml
	-rm -f *.pyc
	-for F in $(COMPLEX_TEST); do $(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR)/$$F $@; done


distclean: clean
	-for F in $(COMPLEX_TEST); do $(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR)/$$F $@; done



################################################################################
# Tests targets
###

build_complex_tests:
	for F in $(COMPLEX_TEST); do $(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR)/$$F; done