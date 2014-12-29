# Copyright (c) 2014 Javier Escalada Gómez
# All rights reserved.

MAKEFILE_FILE = $(abspath $(firstword $(MAKEFILE_LIST)))
MAKEFILE_PATH = $(dir $(MAKEFILE_FILE))
CURRENT_DIR = $(patsubst %/,%,$(MAKEFILE_PATH))
PILF_BASE_DIR = $(abspath $(CURRENT_DIR)/..)/

include ../Makefile.vars
include ../deps/Makefile.vars
include Makefile.vars

DGREP = python $(PILF_BASE_DIR)/tools/dgrep.py



################################################################################
# Main targets
###

.PHONY: all deploy clean distclean

all: deploy

deploy: build
	install -m 755 $(MINION_EXE_TARGET) $(BIN_DIR)
	install -m 644 $(MINION_SHARED_TARGET) $(LIB_DIR)
	install -m 644 $(MINION_STATIC_TARGET) $(LIB_DIR)
	install -d -m 755 $(ETC_DIR)
	install -d -m 755 $(MINION_ETC_DIR)
	install -m 644 $(MINION_SRC_DIR)/boot.lua $(MINION_ETC_DIR)

build: $(VIRTUAL_ENV) $(COMMON_BUILD_DIR) $(MINION_BUILD_DIR) $(MINION_STATIC_TARGET) $(MINION_EXE_TARGET) $(MINION_SHARED_TARGET)

clean:
	-rm $(MINION_STATIC_TARGET) $(MINION_STATIC_OBJS)
	-rm $(MINION_PUBLIC_DECL_SRC) $(MINION_INTERNAL_DECL_SRC) $(MINION_PRIVATE_DECL_SRC)
	-rm $(COMMON_PUBLIC_DECL_SRC) $(COMMON_INTERNAL_DECL_SRC) $(COMMON_PRIVATE_DECL_SRC)
	-rm $(MINION_SHARED_TARGET) $(MINION_SHARED_OBJS)
	-rm $(MINION_EXE_TARGET) $(MINION_EXE_OBJS)

distclean:
	-rm -r $(MINION_BUILD_DIR)



################################################################################
# Common targets
###

$(COMMON_PUBLIC_DECL_SRC): $(addprefix $(COMMON_SRC_DIR)/, $(COMMON_SRCS))
	$(DGREP) -p PILF_PUBLIC -o $@ -s="$(CPPFLAGS) $(filter -I%, $(CFLAGS))" $^

$(COMMON_INTERNAL_DECL_SRC): $(addprefix $(COMMON_SRC_DIR)/, $(COMMON_SRCS))
	$(DGREP) -p PILF_INTERNAL -o $@ -s="$(CPPFLAGS) $(filter -I%, $(CFLAGS))" $^

$(COMMON_PRIVATE_DECL_SRC): $(addprefix $(COMMON_SRC_DIR)/, $(COMMON_SRCS))
	$(DGREP) -p PILF_PRIVATE -o $@ -s="$(CPPFLAGS) $(filter -I%, $(CFLAGS))" $^



################################################################################
# Minion targets
###

$(MINION_EXE_TARGET): $(MINION_EXE_DEPS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter-out %.h, $^) $(MINION_EXE_LDFLAGS) -o $@

$(MINION_SHARED_TARGET):$(MINION_SHARED_DEPS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter-out %.h, $^) $(MINION_SHARED_LDFLAGS) -o $@

$(MINION_STATIC_TARGET):$(MINION_STATIC_DEPS)
	$(AR) crs $@ $(MINION_STATIC_OBJS)

$(MINION_BUILD_DIR):
	mkdir -p $@

$(MINION_PUBLIC_DECL_SRC): $(addprefix $(MINION_SRC_DIR)/, $(MINION_STATIC_SRCS))
	$(DGREP) -p PILF_PUBLIC -o $@ -s="$(CPPFLAGS) $(filter -I%, $(CFLAGS))" $^

$(MINION_INTERNAL_DECL_SRC): $(addprefix $(MINION_SRC_DIR)/, $(MINION_STATIC_SRCS))
	$(DGREP) -p PILF_INTERNAL -o $@ -s="$(CPPFLAGS) $(filter -I%, $(CFLAGS))" $^

$(MINION_PRIVATE_DECL_SRC): $(addprefix $(MINION_SRC_DIR)/, $(MINION_STATIC_SRCS))
	$(DGREP) -p PILF_PRIVATE -o $@ -s="$(CPPFLAGS) $(filter -I%, $(CFLAGS))" $^

################################################################################
# Other targets
###

$(VIRTUAL_ENV):
	mkdir -p $@

$(COMMON_BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
