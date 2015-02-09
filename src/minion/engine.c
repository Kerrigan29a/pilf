#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "minion_internals.h"


#if MINION_TRACE_ENGINE
#define MINION_TRACE_FUNCTION() TRACE_FUNCTION_CALL(ctx, __func__, __FILE__, __LINE__)
#else
#define MINION_TRACE_FUNCTION() ((void)0)
#endif


/*
 * To load compiled Lua modules: https://stackoverflow.com/questions/19416981/running-luajit-object-file-from-c
 */

PILF_PRIVATE int _handle_exec_event(minion_context_t * const ctx,
    minion_engine_event_t * const event)
{
    int result;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_BITS_EQ(MINION_ENGINE_EXEC_EVENT, event->type);

#if MINION_ENABLE_SHOW_VALUE_WITH_EQUAL
    /* Check if line starts with '=' like in the lua interpreter */
    if (event->buffer[0] == '=') {
        char * preffix = "return ";
        char * new_buffer = (char *) pilf_secure_malloc(
            (strlen(event->buffer) + strlen(preffix) + 1) * sizeof(char));
        CHECK_NNULL_R(new_buffer, __LINE__);
        strcpy(new_buffer, preffix);
        strcat(new_buffer, event->buffer + 1);

        /* Replace the old buffer */
        pilf_secure_free(event->buffer, strlen(event->buffer));
        event->buffer = new_buffer;
    }
#endif

    debug(ctx, "(%zu) SCRIPT:\n%s", strlen(event->buffer), event->buffer);

    result = luaL_dostring(ctx->lua_state, event->buffer);

    if (result) {
        const char * msg = lua_tostring(ctx->lua_state, -1);
        EVENTS_NOTIFY_NEW_EVENT(engine, ctx, MINION_ENGINE_DONE_EVENT, msg, strlen(msg));
    } else {
        if (!lua_isnil(ctx->lua_state, -1)) {
            const char *msg = lua_tostring(ctx->lua_state, -1);
            if (msg == NULL) {
                msg = "(error object is not a string)";
            }
            EVENTS_NOTIFY_NEW_EVENT(engine, ctx, MINION_ENGINE_DONE_EVENT, msg, strlen(msg));
            lua_pop(ctx->lua_state, 1);
        } else {
            EVENTS_NOTIFY_NEW_EVENT(engine, ctx, MINION_ENGINE_DONE_EVENT, "DONE", sizeof("DONE"));
        }

    }
    return 0;
}

PILF_INTERNAL int engine_init(minion_context_t * const ctx)
{
    ASSERT_NNULL(ctx);
    ASSERT_TRUE(events_ready(ctx));

    MINION_TRACE_FUNCTION();

    /* Create Lua state*/
    // TODO: Usar lua_newstate para añadirle el Allocator específico
    ctx->lua_state = luaL_newstate();
    ASSERT_NNULL(ctx->lua_state);
    CHECK_NNULL_R(ctx->lua_state, __LINE__);
    luaL_openlibs(ctx->lua_state);

    /* Execute code*/
    // TODO: Init libs here
    if (IS_STR(ctx->file_name)) {
        int result;
        result = luaL_dofile(ctx->lua_state, ctx->file_name);
        if (result) {
            error(ctx, "[engine] %s", lua_tostring(ctx->lua_state, -1));
            return result;
        }

    }
    if (IS_STR(ctx->script)) {
        int result;
        result = luaL_dostring(ctx->lua_state, ctx->script);
        if (result) {
            error(ctx, "[engine] %s", lua_tostring(ctx->lua_state, -1));
            return result;
        }
    }
    /* Bind */
    EVENTS_BIND_NEW_LISTENER(engine, ctx, MINION_ENGINE_EXEC_EVENT, _handle_exec_event);

    return 0;
}

PILF_INTERNAL int engine_finalize(minion_context_t * const ctx)
{
    ASSERT_NNULL(ctx);
    ASSERT_NNULL(ctx->lua_state);

    MINION_TRACE_FUNCTION();

    lua_close(ctx->lua_state);

    return 0;
}
