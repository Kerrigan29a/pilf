#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "minion_internals.h"


#if MINION_TRACE_EVENTS
#define MINION_TRACE_FUNCTION() TRACE_FUNCTION_CALL(ctx, __func__, __FILE__, __LINE__)
#else
#define MINION_TRACE_FUNCTION() ((void)0)
#endif


PILF_PRIVATE minion_listener_t ** _select_list(minion_context_t * const ctx,
    minion_event_type_e event_type)
{
    ASSERT_NNULL(ctx);
    ASSERT_TRUE(event_type > 0);

    MINION_TRACE_FUNCTION();

    if (IS_BITS_IN(MINION_LINK_EVENT, event_type)) {
        return &ctx->link_listeners;
    }
    if (IS_BITS_IN(MINION_ENGINE_EVENT, event_type)) {
        return &ctx->engine_listeners;
    }
    if (IS_BITS_IN(MINION_TIME_EVENT, event_type)) {
        return &ctx->time_listeners;
    }
    CHECK_UNREACHABLE();
}

PILF_INTERNAL void events_bind(minion_context_t * const ctx,
    minion_listener_t * const listener)
{
    minion_listener_t ** listeners;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(listener);
    ASSERT_TRUE(listener->event_type > 0);
    ASSERT_NNULL(listener->handler);

    debug(ctx, "[events] binding event 0x%08X", listener->event_type);

    listeners = _select_list(ctx, listener->event_type);
    DL_APPEND(*listeners, listener);
}

PILF_INTERNAL int events_notify(minion_context_t * const ctx,
    minion_basic_event_t * event)
{
    time_t now;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_TRUE(event->type > 0);

    debug(ctx, "[events] notifying event 0x%08X", event->type);

    now = time(NULL);
    CHECK_TRUE_R(now != -1, __LINE__);
    event->timestamp = now;

    DL_APPEND(ctx->events_queue, event);

    return 0;
}

PILF_PRIVATE int _handle_event(minion_context_t * const ctx,
    minion_basic_event_t * event)
{
    minion_listener_t    ** listeners;
    minion_listener_t    * listener;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_TRUE(event->type > 0);
    ASSERT_TRUE(event->timestamp > 0);

    debug(ctx, "[events] handling event 0x%08X", event->type);

    listeners = _select_list(ctx, event->type);
    DL_FOREACH(*listeners, listener) {
        // NOTE: I use IN and not EQ to allow listeners that handle multiple events
        if (IS_BITS_IN(event->type, listener->event_type)) {
            int result;
            ASSERT_NNULL(listener->handler);
            result = listener->handler(ctx, event);
            CHECK_ZERO_R(result, result);
        }
    }
    return 0;
}

PILF_PRIVATE void _delete_event(minion_context_t * const ctx,
    minion_basic_event_t ** event)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_NNULL(*event);
    ASSERT_TRUE((*event)->type > 0);

    debug(ctx, "[events] deleting event 0x%08X", (*event)->type);

    if (IS_BITS_IN(MINION_LINK_EVENT, (*event)->type)) {
        events_finalize_link_event(ctx, (minion_link_event_t *) *event);
        pilf_secure_free(*event, sizeof(minion_link_event_t));
    } else if (IS_BITS_IN(MINION_ENGINE_EVENT, (*event)->type)) {
        events_finalize_engine_event(ctx, (minion_engine_event_t *) *event);
        pilf_secure_free(*event, sizeof(minion_engine_event_t));
    } else if (IS_BITS_IN(MINION_TIME_EVENT, (*event)->type)) {
        events_finalize_time_event(ctx, (minion_time_event_t *) *event);
        pilf_secure_free(*event, sizeof(minion_time_event_t));
    } else {
        CHECK_UNREACHABLE();
    }
}

PILF_INTERNAL int events_dispatch(minion_context_t * const ctx)
{
    minion_time_event_t clock_event;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);

    events_init_time_event(ctx, &clock_event, MINION_TIME_STEP_EVENT);

    ctx->terminate = false;
    while (!ctx->terminate) {
        events_notify(ctx, (minion_basic_event_t *) &clock_event);

        while(!DL_IS_EMPTY(ctx->events_queue)) {
            int result;
            minion_basic_event_t * current_event = ctx->events_queue;

            result = _handle_event(ctx, current_event);
            CHECK_ZERO_R(result, result);
            DL_DELETE(ctx->events_queue, current_event);
            if (current_event != (minion_basic_event_t *) &clock_event) {
                _delete_event(ctx, &current_event);
            }
        }

        pilf_sleep(ctx->step_time);
    }

    return 0;
}

PILF_INTERNAL bool events_ready(minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);

    return ctx->ready && !ctx->terminate;
}

PILF_INTERNAL int events_init(minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);

    ctx->link_listeners = NULL;
    ctx->engine_listeners = NULL;
    ctx->time_listeners = NULL;
    ctx->step_time = 1;
    ctx->events_queue = NULL;
    ctx->terminate = false;
    ctx->ready = true;
    return 0;
}


#define DELETE_LISTENERS_LIST(l) do { \
    minion_listener_t *listener; \
    minion_listener_t *tmp_listener; \
    DL_FOREACH_SAFE((l), listener, tmp_listener) { \
        DL_DELETE((l), listener); \
        DL_FINALIZE_NODE(listener, minion_listener_t); \
    } \
    ASSERT_NULL(l); \
} while(0);

PILF_INTERNAL int events_finalize(minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);

    DELETE_LISTENERS_LIST(ctx->link_listeners);
    DELETE_LISTENERS_LIST(ctx->engine_listeners);
    DELETE_LISTENERS_LIST(ctx->time_listeners);

    {
        minion_basic_event_t * event;
        minion_basic_event_t * tmp_event;

        DL_FOREACH_SAFE(ctx->events_queue, event, tmp_event) {
            DL_DELETE(ctx->events_queue, event);
            if (!IS_BITS_EQ(MINION_TIME_STEP_EVENT, event->type)) {
                _delete_event(ctx, &event);
            }
        }
        ASSERT_NULL(ctx->events_queue);
    }

    return 0;
}
