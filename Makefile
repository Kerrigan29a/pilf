
# Ashamedly ripped from Luvit Makefile:
# https://github.com/luvit/luvit/blob/master/Makefile

################################################################################
# Common vars
###
VERSION = $(shell git describe --tags)

DEPS_BUILD_DIR = $(shell pwd)/deps_build/
OS_NAME = $(shell uname -s)
MH_NAME = $(shell uname -m)

################################################################################
# LuaJIT build vars
###
LUAJIT_DIR = deps/luajit-2.0/
LUAJIT_VERSION = $(shell git --git-dir $(LUAJIT_DIR).git describe --tags)
LUAJIT_MAKEFLAGS  = PREFIX=$(DEPS_BUILD_DIR)
LUAJIT_MAKEFLAGS += XCFLAGS=-DLUA_LROOT=$(DEPS_BUILD_DIR)
#LUAJIT_MAKEFLAGS  = -g
#LUAJIT_MAKEFLAGS += -DLUAJIT_DISABLE_JIT
LUAJIT_MAKEFLAGS += XCFLAGS+=-DLUAJIT_ENABLE_LUA52COMPAT
#LUAJIT_MAKEFLAGS += -DLUA_USE_APICHECK

################################################################################
# LuaJIT vars for modules compilation
###
#LUAJIT_VAR_ABIVER = $(shell awk '/^abiver/ {split($$0, parts, "="); print(parts[2])}' $(LUAJIT_DIR)/etc/luajit.pc)
#LUAJIT_VAR_INCLUDEDIR = $(DEPS_BUILD_DIR)/include/luajit-2.0
#LUAJIT_VAR_LIBDIR = $(DEPS_BUILD_DIR)/lib
#LUAJIT_VAR_LIBNAME = luajit-$(LUAJIT_VAR_ABIVER)
LUAJIT_VAR_ABIVER = $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/luajit.pc --variable=abiver)
LUAJIT_VAR_INCLUDEDIR = $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/luajit.pc --variable=includedir)/
LUAJIT_VAR_LIBDIR = $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/luajit.pc --variable=libdir)/
LUAJIT_VAR_LIBNAME = $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/luajit.pc --variable=libname)

################################################################################
# LuaJIT vars for main target compilation
###
ifeq ($(OS_NAME),Darwin)
ifeq ($(MH_NAME),x86_64)
STANDALONE_LDFLAGS += -pagezero_size 10000
LDFLAGS += -image_base 100000000
else
LDFLAGS +=
endif
else ifeq ($(OS_NAME),Linux)
LDFLAGS += -Wl,-E
endif
CPPFLAGS += -DLUAJIT_VERSION=\"$(LUAJIT_VERSION)\"
CFLAGS += $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/luajit.pc --cflags)
LDFLAGS += $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/luajit.pc --libs-only-L)
LDFLAGS += -lm
LDFLAGS += -ldl

################################################################################
# LuaUV build vars
###
LIBUV_DIR = deps/libuv/
#LIBUV_VERSION = $(shell git --git-dir $(LIBUV_DIR).git describe --all --long | cut -f 3 -d -)
LIBUV_VERSION = $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/libuv.pc --modversion)
LIBUV_CONFIGUREFLAGS = --prefix=$(DEPS_BUILD_DIR)
#LIBUV_CONFIGUREFLAGS += --enable-dtrace

################################################################################
# LibUV vars for modules compilation
###
LIBUV_VAR_INCLUDEDIR = $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/libuv.pc --variable=includedir)/
LIBUV_VAR_LIBDIR = $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/libuv.pc --variable=libdir)/

################################################################################
# LuaUV vars for main target compilation
###
CPPFLAGS += -DLIBUV_VERSION=\"$(LIBUV_VERSION)\"
CFLAGS += $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/libuv.pc --cflags)
LDFLAGS += $(shell pkg-config $(DEPS_BUILD_DIR)lib/pkgconfig/libuv.pc --libs-only-L)
LDFLAGS += -lpthread
LDFLAGS += -ldl

################################################################################
# Cmockery build vars
###
CMOCKERY_DIR = deps/cmockery2/
CMOCKERY_VERSION = $(shell git --git-dir $(CMOCKERY_DIR).git describe --all --long | cut -f 1 -d -)
CMOCKERY_CONFIGUREFLAGS = --prefix=$(DEPS_BUILD_DIR)
#CMOCKERY_CONFIGUREFLAGS += --enable-gcov

################################################################################
# Cmockery vars for modules compilation
###
CMOCKERY_VAR_INCLUDEDIR = $(DEPS_BUILD_DIR)include/
CMOCKERY_VAR_LIBDIR = $(DEPS_BUILD_DIR)lib/

################################################################################
# LuaSocket build vars
###
LUASOCKET_DIR = deps/luasocket/
LUASOCKET_VERSION = $(shell git --git-dir $(LUASOCKET_DIR).git describe --tags)
#LUASOCKET_MAKEFLAGS += DEBUG=DEBUG
LUASOCKET_MAKEFLAGS  = LUAV=$(LUAJIT_VAR_ABIVER)
ifeq ($(OS_NAME),Darwin)
LUASOCKET_MAKEFLAGS += PLAT=macosx
LUASOCKET_MAKEFLAGS += LUAINC_macosx=$(LUAJIT_VAR_INCLUDEDIR)
LUASOCKET_MAKEFLAGS += LUAPREFIX_macosx=$(DEPS_BUILD_DIR)
else ifeq ($(OS_NAME),Linux)
LUASOCKET_MAKEFLAGS += PLAT=linux
LUASOCKET_MAKEFLAGS += LUAINC_linux=$(LUAJIT_VAR_INCLUDEDIR)
LUASOCKET_MAKEFLAGS += LUAPREFIX_linux=$(DEPS_BUILD_DIR)
endif

################################################################################
# Coxpcall build vars
###
COXPCALL_DIR = deps/coxpcall/
COXPCALL_VERSION = $(shell git --git-dir $(COXPCALL_DIR).git describe --tags)
COXPCALL_MAKEFLAGS = LUA_DIR=$(DEPS_BUILD_DIR)/share/lua/$(LUAJIT_VAR_ABIVER)

################################################################################
# Copas build vars
###
COPAS_DIR = deps/copas/
COPAS_VERSION = $(shell git --git-dir $(COPAS_DIR).git describe --tags)
COPAS_MAKEFLAGS = PREFIX=$(DEPS_BUILD_DIR)

################################################################################
# ConcurrentLua build vars
###
CONCURRENTLUA_DIR = deps/concurrentlua/
CONCURRENTLUA_VERSION = $(shell git --git-dir $(CONCURRENTLUA_DIR).git describe --tags)
CONCURRENTLUA_MAKEFLAGS  = PREFIX=$(DEPS_BUILD_DIR)
CONCURRENTLUA_MAKEFLAGS += LUAVERSION=$(LUAJIT_VAR_ABIVER)
CONCURRENTLUA_MAKEFLAGS += MYCFLAGS=-I$(LUAJIT_VAR_INCLUDEDIR)
CONCURRENTLUA_MAKEFLAGS += MYLDFLAGS=-L$(LUAJIT_VAR_LIBDIR)
CONCURRENTLUA_MAKEFLAGS += LIBLUA=-l$(LUAJIT_VAR_LIBNAME)

################################################################################
# Installation vars
###
#PREFIX ?= /usr/local/
#BIN_DIR ?= $(DEST_DIR)$(PREFIX)/bin/
#INC_DIR ?= $(DEST_DIR)$(PREFIX)/include/luvit/
#LIB_DIR ?= $(DEST_DIR)$(PREFIX)/lib/luvit/

################################################################################
# Build vars
###
DEBUG ?= 1
ifeq ($(DEBUG),1)
CPPFLAGS += -DDEBUG
CFLAGS += -g -Wall -O0
else
CPPFLAGS += -DNDEBUG
endif
CPPFLAGS += -std=c99

TESTS_CFLAGS  = $(CPPFLAGS)
TESTS_CFLAGS += $(CFLAGS)
#TESTS_CFLAGS += -DUNIT_TESTING=1 
TESTS_CFLAGS += -I$(CMOCKERY_VAR_INCLUDEDIR)
TESTS_CFLAGS += -Isrc

SHARED_LDFLAGS += $(LDFLAGS)
STANDALONE_LDFLAGS += $(LDFLAGS)
TESTS_LDFLAGS += $(LDFLAGS)


SRC_DIR = src/
BUILD_DIR = build/
TESTS_DIR = tests/

PUBLIC_DECL = $(SRC_DIR)public_decl.h
INTERNAL_DECL = $(SRC_DIR)internal_decl.h
PRIVATE_DECL = $(SRC_DIR)private_decl.h

STATIC_SRCS  = minion.c server.c
STATIC_OBJS  = $(addprefix $(BUILD_DIR), $(STATIC_SRCS:.c=.o))
STATIC_DEPS  = $(PUBLIC_DECL)
STATIC_DEPS += $(INTERNAL_DECL)
STATIC_DEPS += $(STATIC_OBJS)
STATIC_TARGET = $(BUILD_DIR)libminion.a

SHARED_SRCS  = main_shared.c
SHARED_OBJS  = $(addprefix $(BUILD_DIR), $(SHARED_SRCS:.c=.o))
SHARED_DEPS  = $(SHARED_OBJS)
SHARED_DEPS += $(STATIC_TARGET)
SHARED_DEPS += $(LUAJIT_VAR_LIBDIR)lib$(LUAJIT_VAR_LIBNAME).a
SHARED_DEPS += $(LIBUV_VAR_LIBDIR)libuv.a
ifeq ($(OS_NAME),Darwin)
SHARED_TARGET = $(BUILD_DIR)libminion.dylib
else
SHARED_TARGET = $(BUILD_DIR)libminion.so
endif

STANDALONE_SRCS  = main_standalone.c
STANDALONE_OBJS  = $(addprefix $(BUILD_DIR), $(STANDALONE_SRCS:.c=.o))
STANDALONE_DEPS  = $(STANDALONE_OBJS)
STANDALONE_DEPS += $(STATIC_TARGET)
STANDALONE_DEPS += $(LUAJIT_VAR_LIBDIR)lib$(LUAJIT_VAR_LIBNAME).a
STANDALONE_DEPS += $(LIBUV_VAR_LIBDIR)libuv.a
STANDALONE_TARGET = $(BUILD_DIR)minion

TESTS_SRCS = $(wildcard $(TESTS_DIR)*.c)
TESTS_DEPS = $(STATIC_TARGET)
TESTS_DEPS += $(LUAJIT_VAR_LIBDIR)lib$(LUAJIT_VAR_LIBNAME).a
TESTS_DEPS += $(LIBUV_VAR_LIBDIR)libuv.a
TESTS_TARGETS = $(basename $(TESTS_SRCS))


################################################################################
# Build main program
###
.PHONY: all deps minion clean test

all: deps minion

minion: $(BUILD_DIR) $(STATIC_TARGET) $(SHARED_TARGET) $(STANDALONE_TARGET)

$(BUILD_DIR):
	mkdir $@

$(SHARED_TARGET): $(SHARED_DEPS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SHARED_LDFLAGS)  -shared -o $@ $^

$(STATIC_TARGET): $(STATIC_DEPS)
	$(AR) crv $@ $(STATIC_OBJS)

$(STANDALONE_TARGET): $(STANDALONE_DEPS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(STANDALONE_LDFLAGS) -o $@ $^

$(PUBLIC_DECL): $(addprefix $(SRC_DIR), $(STATIC_SRCS))
	python tools/declarations.py $^ -p PILF_PUBLIC -o $@

$(INTERNAL_DECL): $(addprefix $(SRC_DIR), $(STATIC_SRCS))
	python tools/declarations.py $^ -p PILF_INTERNAL -o $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

################################################################################
# Tests
###

check: CPPFLAGS += -DUNIT_TESTING=1
check: clean $(INTERNAL_DECL) $(PUBLIC_DECL) $(PRIVATE_DECL) \
	$(TESTS_TARGETS) run_tests

$(PRIVATE_DECL): $(addprefix $(SRC_DIR), $(STATIC_SRCS))
	python tools/declarations.py $^ -p PILF_PRIVATE -o $@

run_tests: $(TESTS_TARGETS)
	clear
	$(TESTS_TARGETS)

$(basename $(TESTS_DIR)%.o): $(TESTS_DIR)%.c $(CMOCKERY_VAR_LIBDIR)$(CMOCKERY_LIBNAME) $(TESTS_DEPS)
	$(CC) $(TESTS_CFLAGS) $(TESTS_LDFLAGS) -o $@ $^

################################################################################
# Clean
###

clean:
	-rm $(STATIC_TARGET) $(STATIC_OBJS)
	-rm $(PUBLIC_DECL) $(INTERNAL_DECL) $(PRIVATE_DECL)
	-rm $(SHARED_TARGET) $(SHARED_OBJS)
	-rm $(STANDALONE_TARGET) $(STANDALONE_OBJS)

################################################################################
# Build dependencies
###
deps: $(DEPS_BUILD_DIR) build_luajit build_libuv build_cmockery build_luasocket \
	build_coxpcall build_copas build_concurrentlua

$(LUAJIT_DIR)Makefile:
	git submodule update --init $(LUAJIT_DIR)

$(LIBUV_DIR)Makefile.am:
	git submodule update --init $(LIBUV_DIR)

$(LIBUV_DIR)configure: $(LIBUV_DIR)Makefile.am
	cd $(LIBUV_DIR) && ./autogen.sh
	cd $(LIBUV_DIR) && chmod +x configure

$(CMOCKERY_DIR)CMakeList.txt:
	git submodule update --init $(CMOCKERY_DIR)

$(CMOCKERY_DIR)$(CMOCKERY_LIBNAME): $(CMOCKERY_DIR)src/$(CMOCKERY_SRCS)
	$(CC) $(CMOCKERY_CFLAGS) -c -o $(CMOCKERY_DIR)/src/$(CMOCKERY_OBJS) $^
	$(AR) crv $@ $(CMOCKERY_DIR)/src/$(CMOCKERY_OBJS)

$(LUASOCKET_DIR)makefile:
	git submodule update --init $(LUASOCKET_DIR)

$(COXPCALL_DIR)Makefile:
	git submodule update --init $(COXPCALL_DIR)

$(COPAS_DIR)Makefile:
	git submodule update --init $(COPAS_DIR)

$(CONCURRENTLUA_DIR)Makefile:
	git submodule update --init $(CONCURRENTLUA_DIR)

$(DEPS_BUILD_DIR):
	mkdir $@

build_luajit: $(LUAJIT_DIR)Makefile
	touch -c $(LUAJIT_DIR)src/*.h
	$(MAKE) -C $(LUAJIT_DIR) $(LUAJIT_MAKEFLAGS)
	$(MAKE) -C $(LUAJIT_DIR) $(LUAJIT_MAKEFLAGS) install

build_libuv: $(LIBUV_DIR)configure
	cd $(LIBUV_DIR) && ./configure $(LIBUV_CONFIGUREFLAGS)
	touch -c $(LIBUV_DIR)src/*.h 
	touch -c $(LIBUV_DIR)src/unix/*.h
	$(MAKE) -C $(LIBUV_DIR)
	$(MAKE) -C $(LIBUV_DIR) install

build_cmockery: $(CMOCKERY_DIR)configure
	cd $(LIBUV_DIR) && ./autogen.sh && ./configure $(CMOCKERY_CONFIGUREFLAGS)
	touch -c $(CMOCKERY_DIR)src/cmockery/*.h
	$(MAKE) -C $(CMOCKERY_DIR)
	$(MAKE) -C $(CMOCKERY_DIR) install


build_luasocket: $(LUASOCKET_DIR)makefile
	touch -c $(LUASOCKET_DIR)src/*.h
	$(MAKE) -C $(LUASOCKET_DIR) $(LUASOCKET_MAKEFLAGS)
	$(MAKE) -C $(LUASOCKET_DIR) $(LUASOCKET_MAKEFLAGS) install

build_coxpcall: $(COXPCALL_DIR)Makefile
	$(MAKE) -C $(COXPCALL_DIR) $(COXPCALL_MAKEFLAGS) install

build_copas: $(COPAS_DIR)Makefile
	$(MAKE) -C $(COPAS_DIR) $(COPAS_MAKEFLAGS) install

build_concurrentlua: $(CONCURRENTLUA_DIR)Makefile
	touch -c $(CONCURRENTLUA_DIR)src/*/*.h
	$(MAKE) -C $(CONCURRENTLUA_DIR) $(CONCURRENTLUA_MAKEFLAGS)
	$(MAKE) -C $(CONCURRENTLUA_DIR) $(CONCURRENTLUA_MAKEFLAGS) install
