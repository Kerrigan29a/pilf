# Copyright (c) 2014-2015 Javier Escalada Gómez
# All rights reserved.

MAKEFILE_FILE = $(abspath $(firstword $(MAKEFILE_LIST)))
MAKEFILE_PATH = $(dir $(MAKEFILE_FILE))
CURRENT_DIR = $(patsubst %/,%,$(MAKEFILE_PATH))
PILF_BASE_DIR = $(abspath $(CURRENT_DIR)/..)/

include ../Makefile.vars
include Makefile.vars



################################################################################
# Main targets
###

.PHONY: all clean distclean

all: $(VIRTUAL_ENV) deploy_luajit deploy_nanomsg deploy_cmockery deploy_uthash

clean: clean_luajit clean_nanomsg clean_cmockery clean_uthash

distclean: distclean_luajit distclean_nanomsg distclean_cmockery distclean_uthash



################################################################################
# Common targets
###

$(VIRTUAL_ENV):
	mkdir -p $@



################################################################################
# LuaJIT targets
###

deploy_luajit: build_luajit
	$(MAKE) -C $(LUAJIT_DIR) $(LUAJIT_MAKEFLAGS) install

ifeq ($(FORCE_BUILD),1)
build_luajit: force_build_luajit
endif

force_build_luajit:
	touch -c $(LUAJIT_DIR)/src/*.h

build_luajit: $(LUAJIT_DIR)/Makefile
	$(MAKE) -C $(LUAJIT_DIR) $(LUAJIT_MAKEFLAGS)

$(LUAJIT_DIR)/Makefile:
	git submodule update --init $(LUAJIT_DIR)

clean_luajit: $(LUAJIT_DIR)/Makefile
	$(MAKE) -C $(LUAJIT_DIR) $(LUAJIT_MAKEFLAGS) clean

distclean_luajit: clean_luajit



################################################################################
# Nanomsg targets
###

deploy_nanomsg: build_nanomsg
	$(MAKE) -C $(NANOMSG_DIR) install

ifeq ($(FORCE_BUILD),1)
build_nanomsg: force_build_nanomsg
endif

force_build_nanomsg:
	touch -c $(NANOMSG_DIR)src/*.h
	touch -c $(NANOMSG_DIR)src/unix/*.h
	touch -c $(NANOMSG_DIR)/Makefile.am

build_nanomsg: $(NANOMSG_DIR)/Makefile
	$(MAKE) -C $(NANOMSG_DIR)

$(NANOMSG_DIR)/Makefile: $(NANOMSG_DIR)/configure
	cd $(NANOMSG_DIR) && ./configure $(NANOMSG_CONFIGUREFLAGS)

$(NANOMSG_DIR)/configure: $(NANOMSG_DIR)/Makefile.am
	cd $(NANOMSG_DIR) && ./autogen.sh
	cd $(NANOMSG_DIR) && chmod +x configure

$(NANOMSG_DIR)/Makefile.am:
	git submodule update --init $(NANOMSG_DIR)

clean_nanomsg: $(NANOMSG_DIR)/Makefile
	$(MAKE) -C $(NANOMSG_DIR) clean

distclean_nanomsg: $(NANOMSG_DIR)/Makefile
	$(MAKE) -C $(NANOMSG_DIR) distclean



################################################################################
# Cmockery targets
###

deploy_cmockery: build_cmockery
	$(MAKE) -C $(CMOCKERY_DIR) install

ifeq ($(FORCE_BUILD),1)
build_cmockery: force_build_cmockery
endif

force_build_cmockery:
	touch -c $(CMOCKERY_DIR)src/cmockery/*.h
	touch -c $(CMOCKERY_DIR)/configure.ac

build_cmockery: $(CMOCKERY_DIR)/Makefile
	$(MAKE) -C $(CMOCKERY_DIR)

$(CMOCKERY_DIR)/Makefile: $(CMOCKERY_DIR)/configure
	cd $(CMOCKERY_DIR) && ./configure $(CMOCKERY_CONFIGUREFLAGS)

$(CMOCKERY_DIR)/configure: $(CMOCKERY_DIR)/configure.ac
	cd $(CMOCKERY_DIR) && ./autogen.sh

$(CMOCKERY_DIR)/configure.ac:
	git submodule update --init $(CMOCKERY_DIR)

clean_cmockery: $(CMOCKERY_DIR)/Makefile
	$(MAKE) -C $(CMOCKERY_DIR) clean

distclean_cmockery: $(CMOCKERY_DIR)/Makefile
	$(MAKE) -C $(CMOCKERY_DIR) distclean



################################################################################
# Uthash targets
###

deploy_uthash: $(UTHASH_DIR)/README.md
	cp -R $(UTHASH_DIR)/src $(UTHASH_INCLUDEDIR)

$(UTHASH_DIR)/README.md:
	git submodule update --init $(UTHASH_DIR)

clean_uthash:

distclean_uthash:
