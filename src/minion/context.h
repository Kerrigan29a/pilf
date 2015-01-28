#ifndef __PILF_MINION__CONTEXT_H__
#define __PILF_MINION__CONTEXT_H__


#ifndef PILF_PRIVATE
#define PILF_PRIVATE static
#endif

#include <stdbool.h>
#include <stdio.h>
#include "../common/check.h"

#ifdef __PILF_MINION__MINION_INTERNALS_H__
#include <luajit-2.0/lua.h>

#include "events.h"
#include "link.h"
#endif


typedef struct _context {
    /* Public */
    char                    * file_name;
    char                    * script;
    char                    * master_url;
    char                    * log_prefix;
    FILE                    * log_file;

    /* Events private */
    bool                    terminate;
    bool                    ready;
    unsigned                step_time;
#ifdef __PILF_MINION__MINION_INTERNALS_H__
    minion_listener_t       * link_listeners;
    minion_listener_t       * engine_listeners;
    minion_listener_t       * time_listeners;
    minion_basic_event_t    * events_queue;
#else
    void                    * link_listeners;
    void                    * engine_listeners;
    void                    * time_listeners;
    void                    * events_queue;
#endif

    /* Link private */
#ifdef __PILF_MINION__MINION_INTERNALS_H__
    minion_socket_t         * sockets;
#else
    void                    * sockets;
#endif

    /* Engine private */
#ifdef __PILF_MINION__MINION_INTERNALS_H__
    lua_State               * lua_state;
#else
    void                    * lua_state;
#endif

} minion_context_t;

#define minion_context_init(ctx) do { \
    ASSERT_NNULL(ctx); \
    (ctx)->file_name = NULL; \
    (ctx)->script = NULL; \
    (ctx)->master_url = NULL; \
    (ctx)->log_prefix = NULL; \
    (ctx)->log_file = stderr; \
} while(0)


#endif /* __PILF_MINION__CONTEXT_H__ */