# Copyright (c) 2014-2015 Javier Escalada Gómez
# All rights reserved.

MAKEFILE_FILE = $(abspath $(firstword $(MAKEFILE_LIST)))
MAKEFILE_PATH = $(dir $(MAKEFILE_FILE))
CURRENT_DIR = $(patsubst %/,%,$(MAKEFILE_PATH))
PILF_BASE_DIR = $(abspath $(CURRENT_DIR))/

include Makefile.vars


################################################################################
# Main targets
###

.PHONY: all test check clean distclean update

all:
	$(MAKE) $(MAKEFLAGS) -C $(DEPS_DIR)
	$(MAKE) $(MAKEFLAGS) -C $(SRC_DIR)

test:
	$(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR)

check: test

clean:
	$(MAKE) $(MAKEFLAGS) -C $(SRC_DIR) clean
	$(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR) clean

distclean:
	$(MAKE) $(MAKEFLAGS) -C $(DEPS_DIR) clean
	$(MAKE) $(MAKEFLAGS) -C $(DEPS_DIR) distclean
	$(MAKE) $(MAKEFLAGS) -C $(SRC_DIR) distclean
	$(MAKE) $(MAKEFLAGS) -C $(TESTS_DIR) distclean
	-rm -r $(VIRTUAL_ENV)
	-rm -r $(BUILD_DIR)

update:
	git submodule foreach git pull origin master