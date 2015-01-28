# Copyright (c) 2015 Javier Escalada Gómez
# All rights reserved.

cmake_minimum_required (VERSION 2.8)
include(ExternalProject)

project (minion C)

################################################################################

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set (MINION_PRIVATE_DECL ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/private_decl.h)
set (MINION_INTERNAL_DECL ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/internal_decl.h)
set (MINION_PUBLIC_DECL ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/public_decl.h)
set (COMMON_PRIVATE_DECL ${CMAKE_CURRENT_SOURCE_DIR}/src/common/private_decl.h)
set (COMMON_INTERNAL_DECL ${CMAKE_CURRENT_SOURCE_DIR}/src/common/internal_decl.h)
set (COMMON_PUBLIC_DECL ${CMAKE_CURRENT_SOURCE_DIR}/src/common/public_decl.h)

set (MINION_STATIC_CODES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/engine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/events.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/link.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/message_workflow.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/minion.c
    )

set (COMMON_STATIC_CODES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/string.c
    )

set (MINION_STATIC_SOURCES
    ${MINION_STATIC_CODES}
    ${COMMON_STATIC_CODES}
    ${MINION_INTERNAL_DECL}
    ${MINION_PRIVATE_DECL}
    ${MINION_PUBLIC_DECL}
    ${COMMON_INTERNAL_DECL}
    ${COMMON_PRIVATE_DECL}
    ${COMMON_PUBLIC_DECL}
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/context.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/engine.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/engine_events.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/events.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/link.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/link_events.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/log.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/memory.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/message_workflow.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/minion.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/minion_config.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/minion_internals.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/sleep.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/time_events.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/utlist_utils.h
    )


set (MINION_SHARE_SOURCES
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/minion.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/main_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/main_shared.c
    )

set (MINION_EXE_SOURCES
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/minion.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/main_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/main_standalone.c
    )

set(VIRTUAL_ENV ${CMAKE_CURRENT_SOURCE_DIR}/venv)



################################################################################
# Build deps
###

set(CMOCKERY2_CONFIGURE_OPTIONS --prefix=${VIRTUAL_ENV})
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CMOCKERY2_CONFIGURE_OPTIONS ${CMOCKERY2_CONFIGURE_OPTIONS} --enable-gcov)
endif ()

ExternalProject_Add(
  cmockery2
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/cmockery2
  CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps/cmockery2/configure ${CMOCKERY2_CONFIGURE_OPTIONS}
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps/cmockery2
  BUILD_COMMAND make
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND make install
)



set(LUAJIT2_MAKE_OPTIONS PREFIX=${VIRTUAL_ENV})
set(LUAJIT2_MAKE_OPTIONS ${LUAJIT2_MAKE_OPTIONS} XCFLAGS=-DLUA_LROOT=${VIRTUAL_ENV})
set(LUAJIT2_MAKE_OPTIONS ${LUAJIT2_MAKE_OPTIONS} XCFLAGS+=-DLUAJIT_ENABLE_LUA52COMPAT)
#set(LUAJIT2_MAKE_OPTIONS ${LUAJIT2_MAKE_OPTIONS} XCFLAGS+=-DLUAJIT_DISABLE_JIT)
#set(LUAJIT2_MAKE_OPTIONS ${LUAJIT2_MAKE_OPTIONS} XCFLAGS+=-DLUA_USE_APICHECK)
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(LUAJIT2_MAKE_OPTIONS ${LUAJIT2_MAKE_OPTIONS} XCFLAGS+=-g)
endif ()

ExternalProject_Add(
  luajit2
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/luajit-2.0
  CONFIGURE_COMMAND ""
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps/luajit-2.0
  BUILD_COMMAND make ${LUAJIT2_MAKE_OPTIONS}
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND make install PREFIX=${VIRTUAL_ENV}
)



set(NANOMSG_CONFIGURE_OPTIONS --prefix=${VIRTUAL_ENV})
#set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --enable-doc)
set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --disable-shared)
#set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --disable-symlinks)
#set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --disable-nanocat)
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --enable-debug)
endif ()

ExternalProject_Add(
  nanomsg
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/nanomsg
  CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps/nanomsg/configure ${NANOMSG_CONFIGURE_OPTIONS}
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps/nanomsg
  BUILD_COMMAND make
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND make install
)



file(MAKE_DIRECTORY ${VIRTUAL_ENV}/include/uthash)
file(INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/deps/uthash/src/ DESTINATION ${VIRTUAL_ENV}/include/uthash FILES_MATCHING PATTERN *.h)



################################################################################
# Build automatic declaration files
###

set(DGREP python ${CMAKE_CURRENT_SOURCE_DIR}/tools/dgrep.py)

add_custom_command (
  OUTPUT ${MINION_PRIVATE_DECL}
  COMMAND ${DGREP} -p PILF_PRIVATE -o ${MINION_PRIVATE_DECL} -s='-I ${VIRTUAL_ENV}/include' ${MINION_STATIC_CODES}
  )

add_custom_command (
  OUTPUT ${MINION_INTERNAL_DECL}
  COMMAND ${DGREP} -p PILF_INTERNAL -o ${MINION_INTERNAL_DECL} -s='-I ${VIRTUAL_ENV}/include' ${MINION_STATIC_CODES}
  )

add_custom_command (
  OUTPUT ${MINION_PUBLIC_DECL}
  COMMAND ${DGREP} -p PILF_PUBLIC -o ${MINION_PUBLIC_DECL} -s='-I ${VIRTUAL_ENV}/include' ${MINION_STATIC_CODES}
  )

add_custom_command (
  OUTPUT ${COMMON_PRIVATE_DECL}
  COMMAND ${DGREP} -p PILF_PRIVATE -o ${COMMON_PRIVATE_DECL} -s='-I ${VIRTUAL_ENV}/include' ${COMMON_STATIC_CODES}
  )

add_custom_command (
  OUTPUT ${COMMON_INTERNAL_DECL}
  COMMAND ${DGREP} -p PILF_INTERNAL -o ${COMMON_INTERNAL_DECL} -s='-I ${VIRTUAL_ENV}/include' ${COMMON_STATIC_CODES}
  )

add_custom_command (
  OUTPUT ${COMMON_PUBLIC_DECL}
  COMMAND ${DGREP} -p PILF_PUBLIC -o ${COMMON_PUBLIC_DECL} -s='-I ${VIRTUAL_ENV}/include' ${COMMON_STATIC_CODES}
  )



################################################################################
# Build minion
###

include_directories(${VIRTUAL_ENV}/include)

add_library (minion_obj OBJECT ${MINION_STATIC_SOURCES})
add_library (minion SHARED ${MINION_SHARED_SOURCES} $<TARGET_OBJECTS:minion_obj>)
target_link_libraries(minion
        ${VIRTUAL_ENV}/lib/libcmockery.a
        ${VIRTUAL_ENV}/lib/libluajit-5.1.a
        ${VIRTUAL_ENV}/lib/libnanomsg.a
        )
add_executable(minion_standalone ${MINION_EXE_SOURCES} $<TARGET_OBJECTS:minion_obj>)
target_link_libraries(minion_standalone
        ${VIRTUAL_ENV}/lib/libcmockery.a
        ${VIRTUAL_ENV}/lib/libluajit-5.1.a
        ${VIRTUAL_ENV}/lib/libnanomsg.a
        )

install (TARGETS minion_standalone RUNTIME DESTINATION ${VIRTUAL_ENV}/bin)
install (TARGETS minion LIBRARY DESTINATION ${VIRTUAL_ENV}/lib)
