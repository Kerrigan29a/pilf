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
    )


set (MINION_SHARE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/main_shared.c
    )

set (MINION_EXE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/main_standalone.c
    )

set(VIRTUAL_ENV ${CMAKE_CURRENT_SOURCE_DIR}/venv)



################################################################################
# Build deps
###

set(CMOCKERY2_CONFIGURE_OPTIONS --prefix=${VIRTUAL_ENV})
set(CMOCKERY2_CONFIGURE_OPTIONS ${CMOCKERY2_CONFIGURE_OPTIONS} --with-pic)
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CMOCKERY2_CONFIGURE_OPTIONS ${CMOCKERY2_CONFIGURE_OPTIONS} --enable-gcov)
endif ()

ExternalProject_Add( cmockery2
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/cmockery2
  CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps/cmockery2/autogen.sh && ${CMAKE_CURRENT_SOURCE_DIR}/deps/cmockery2/configure ${CMOCKERY2_CONFIGURE_OPTIONS}
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps/cmockery2
  BUILD_COMMAND make
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND make install
)



set(LUA_MAKE_OPTIONS INSTALL_TOP=${VIRTUAL_ENV})
set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} MYCFLAGS=-DLUA_ROOT=${VIRTUAL_ENV})
set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} MYCFLAGS+=-DLUA_USE_POSIX)
set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} MYCFLAGS+=-DLUA_USE_DLOPEN)
set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} MYCFLAGS+=-fPIC)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} CC=cc)
else ()
  set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} MYLDFLAGS=-Wl,-E)
  set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} MYLIBS=-ldl)
endif ()
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(LUA_MAKE_OPTIONS ${LUA_MAKE_OPTIONS} MYCFLAGS+=-g)
endif ()

ExternalProject_Add( lua
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/lua
  CONFIGURE_COMMAND ""
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps/lua
  BUILD_COMMAND make generic ${LUA_MAKE_OPTIONS}
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND make install INSTALL_TOP=${VIRTUAL_ENV}
)



set(NANOMSG_CONFIGURE_OPTIONS --prefix=${VIRTUAL_ENV})
set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --with-pic)
#set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --enable-doc)
set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --disable-shared)
#set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --disable-symlinks)
#set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --disable-nanocat)
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(NANOMSG_CONFIGURE_OPTIONS ${NANOMSG_CONFIGURE_OPTIONS} --enable-debug)
endif ()

ExternalProject_Add( nanomsg
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/nanomsg
  CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps/nanomsg/autogen.sh && CFLAGS=-fPIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/nanomsg/configure ${NANOMSG_CONFIGURE_OPTIONS}
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps/nanomsg
  BUILD_COMMAND make
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND make install
)



ExternalProject_Add( uthash
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/uthash
  CONFIGURE_COMMAND ""
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps/uthash
  BUILD_COMMAND ""
  INSTALL_COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/deps/uthash/src ${VIRTUAL_ENV}/include/ && mv ${VIRTUAL_ENV}/include/src ${VIRTUAL_ENV}/include/uthash
)



################################################################################
# Build automatic declaration files
###

set(DGREP python ${CMAKE_CURRENT_SOURCE_DIR}/tools/dgrep.py)

add_custom_command (
  OUTPUT ${MINION_PRIVATE_DECL}
  COMMAND ${DGREP} -p PILF_PRIVATE -o ${MINION_PRIVATE_DECL} -s='-I ${VIRTUAL_ENV}/include' ${MINION_STATIC_CODES}
  DEPENDS cmockery2
  DEPENDS lua
  DEPENDS nanomsg
  DEPENDS uthash
  )

add_custom_command (
  OUTPUT ${MINION_INTERNAL_DECL}
  COMMAND ${DGREP} -p PILF_INTERNAL -o ${MINION_INTERNAL_DECL} -s='-I ${VIRTUAL_ENV}/include' ${MINION_STATIC_CODES}
  DEPENDS cmockery2
  DEPENDS lua
  DEPENDS nanomsg
  DEPENDS uthash
  )

add_custom_command (
  OUTPUT ${MINION_PUBLIC_DECL}
  COMMAND ${DGREP} -p PILF_PUBLIC -o ${MINION_PUBLIC_DECL} -s='-I ${VIRTUAL_ENV}/include' ${MINION_STATIC_CODES}
  DEPENDS cmockery2
  DEPENDS lua
  DEPENDS nanomsg
  DEPENDS uthash
  )

add_custom_command (
  OUTPUT ${COMMON_PRIVATE_DECL}
  COMMAND ${DGREP} -p PILF_PRIVATE -o ${COMMON_PRIVATE_DECL} -s='-I ${VIRTUAL_ENV}/include' ${COMMON_STATIC_CODES}
  DEPENDS cmockery2
  DEPENDS lua
  DEPENDS nanomsg
  DEPENDS uthash
  )

add_custom_command (
  OUTPUT ${COMMON_INTERNAL_DECL}
  COMMAND ${DGREP} -p PILF_INTERNAL -o ${COMMON_INTERNAL_DECL} -s='-I ${VIRTUAL_ENV}/include' ${COMMON_STATIC_CODES}
  DEPENDS cmockery2
  DEPENDS lua
  DEPENDS nanomsg
  DEPENDS uthash
  )

add_custom_command (
  OUTPUT ${COMMON_PUBLIC_DECL}
  COMMAND ${DGREP} -p PILF_PUBLIC -o ${COMMON_PUBLIC_DECL} -s='-I ${VIRTUAL_ENV}/include' ${COMMON_STATIC_CODES}
  DEPENDS cmockery2
  DEPENDS lua
  DEPENDS nanomsg
  DEPENDS uthash
  )



################################################################################
# Build minion
###

include_directories(${VIRTUAL_ENV}/include)

add_library (minion_obj OBJECT ${MINION_STATIC_SOURCES})
set_property(TARGET minion_obj PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library (minion SHARED ${MINION_SHARED_SOURCES} $<TARGET_OBJECTS:minion_obj>)
set_property(TARGET minion PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)
target_link_libraries(minion
        ${VIRTUAL_ENV}/lib/libcmockery.a
        ${VIRTUAL_ENV}/lib/liblua.a
        ${VIRTUAL_ENV}/lib/libnanomsg.a
        )
target_link_libraries(minion m)

add_executable(minion_standalone ${MINION_EXE_SOURCES} $<TARGET_OBJECTS:minion_obj>)
target_link_libraries(minion_standalone
        ${VIRTUAL_ENV}/lib/libcmockery.a
        ${VIRTUAL_ENV}/lib/liblua.a
        ${VIRTUAL_ENV}/lib/libnanomsg.a
        )
target_link_libraries(minion_standalone m)


install (TARGETS minion_standalone RUNTIME DESTINATION ${VIRTUAL_ENV}/bin)
install (TARGETS minion LIBRARY DESTINATION ${VIRTUAL_ENV}/lib)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/minion/boot.lua DESTINATION ${VIRTUAL_ENV}/etc/pilf/minion)
