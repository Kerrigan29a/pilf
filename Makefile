# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

MAKEFILE_FILE = $(abspath $(firstword $(MAKEFILE_LIST)))
MAKEFILE_PATH = $(dir $(MAKEFILE_FILE))
CURRENT_DIR = $(patsubst %/,%,$(MAKEFILE_PATH))
PILF_BASE_DIR = $(abspath $(CURRENT_DIR))/

include Makefile.vars


################################################################################
# Main targets
###

.PHONY: all test check clean distclean

all:
	$(MAKE) $(MAKEFLAGS) -C $(DEPS_DIR)
	$(MAKE) $(MAKEFLAGS) -C $(SRC_DIR)

test:
	$(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR)

check: test

clean:
	$(MAKE) $(MAKEFLAGS) -C $(DEPS_DIR) $@
	$(MAKE) $(MAKEFLAGS) -C $(SRC_DIR) $@
	$(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR) $@

distclean:
	$(MAKE) $(MAKEFLAGS) -C $(DEPS_DIR) $@
	$(MAKE) $(MAKEFLAGS) -C $(SRC_DIR) $@
	$(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR) $@
	-rm -r $(VIRTUAL_ENV)
	-rm -r $(BUILD_DIR)
