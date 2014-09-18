# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

MAKEFILE_FILE = $(abspath $(firstword $(MAKEFILE_LIST)))
MAKEFILE_PATH = $(dir $(MAKEFILE_FILE))
CURRENT_DIR = $(patsubst %/,%,$(MAKEFILE_PATH))
PILF_BASE_DIR = $(abspath $(CURRENT_DIR)/..)/

include ../Makefile.vars
include ../deps/Makefile.vars
include Makefile.vars

DECL_EXTRACTOR = python $(PILF_BASE_DIR)/tools/declarations.py



################################################################################
# Main targets
###

.PHONY: all deploy clean distclean

all: deploy

clean:
	-rm $(MINION_STATIC_TARGET) $(MINION_STATIC_OBJS)
	-rm $(MINION_PUBLIC_DECL_SRC) $(MINION_INTERNAL_DECL_SRC) $(MINION_PRIVATE_DECL_SRC)
	-rm $(MINION_SHARED_TARGET) $(MINION_SHARED_OBJS)
	-rm $(MINION_EXE_TARGET) $(MINION_EXE_OBJS)

distclean:
	-rm -r $(MINION_BUILD_DIR)



################################################################################
# Common targets
###

deploy: $(VIRTUAL_ENV) $(MINION_BUILD_DIR) $(MINION_STATIC_TARGET) $(MINION_EXE_TARGET) $(MINION_SHARED_TARGET)
	install -m 755 $(MINION_EXE_TARGET) $(BIN_DIR)
	install -m 644 $(MINION_SHARED_TARGET) $(LIB_DIR)
	install -m 644 $(MINION_STATIC_TARGET) $(LIB_DIR)
	install -d -m 755 $(ETC_DIR)
	install -m 644 $(MINION_SRC_DIR)/boot.lua $(MINION_ETC_DIR)



################################################################################
# Minion targets
###

$(MINION_EXE_TARGET): $(MINION_EXE_DEPS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(MINION_EXE_LDFLAGS) -o $@ $^

$(MINION_SHARED_TARGET):$(MINION_SHARED_DEPS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(MINION_SHARED_LDFLAGS) -o $@ $^

$(MINION_STATIC_TARGET):$(MINION_STATIC_DEPS)
	$(AR) crv $@ $(MINION_STATIC_OBJS)

$(MINION_BUILD_DIR):
	mkdir -p $@

$(VIRTUAL_ENV):
	mkdir -p $@

$(MINION_PUBLIC_DECL_SRC): $(addprefix $(MINION_SRC_DIR)/, $(MINION_STATIC_SRCS))
	$(DECL_EXTRACTOR) $^ -p PILF_PUBLIC -o $@

$(MINION_INTERNAL_DECL_SRC): $(addprefix $(MINION_SRC_DIR)/, $(MINION_STATIC_SRCS))
	$(DECL_EXTRACTOR) $^ -p PILF_INTERNAL -o $@

$(MINION_PRIVATE_DECL_SRC): $(addprefix $(MINION_SRC_DIR)/, $(MINION_STATIC_SRCS))
	$(DECL_EXTRACTOR) $^ -p PILF_PRIVATE -o $@

$(MINION_BUILD_DIR)%.o: $(MINION_SRC_DIR)%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
