#include <string.h>

#include "minion_internals.h"


#if MINION_TRACE_MESSAGE_WORKFLOW
#define MINION_TRACE_FUNCTION() TRACE_FUNCTION_CALL(ctx, __func__, __FILE__, __LINE__)
#else
#define MINION_TRACE_FUNCTION() ((void)0)
#endif


PILF_PRIVATE int _handle_time_step_event(minion_context_t * const ctx,
    minion_time_event_t * const event)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_BITS_IN(MINION_TIME_STEP_EVENT, event->type);

    return link_poll(ctx);
}

PILF_PRIVATE int _handle_link_inmsg_event(minion_context_t * const ctx,
    minion_link_event_t * const event)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_BITS_IN(MINION_LINK_INMSG_EVENT, event->type);
    ASSERT_TRUE(event->fd >= 0);
    ASSERT_STR(event->msg);

    debug(ctx, "[events] (%zu) IN_MSG:\n%s", strlen(event->msg), event->msg);

    if (event->fd == link_get_master_socket(ctx)->fd) {
        EVENTS_NOTIFY_NEW_EVENT(engine, ctx, MINION_ENGINE_EXEC_EVENT, event->msg, strlen(event->msg));
    } else {
        //XXX: De momento los mensajes que no son de Master, se filtran
        info(ctx, "[messages_workflow] socket %d -> %s", event->fd, event->msg);
    }

    return 0;
}

PILF_PRIVATE int _handle_engine_done_event(minion_context_t * const ctx,
    minion_engine_event_t * const event)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(event);
    ASSERT_BITS_IN(MINION_ENGINE_DONE_EVENT, event->type);
    ASSERT_STR(event->buffer);

    //XXX: En este momento solo ejecuto codigo de Master
    EVENTS_NOTIFY_NEW_EVENT(link, ctx, MINION_LINK_OUTMSG_EVENT,
        link_get_master_socket(ctx)->fd, event->buffer, strlen(event->buffer));

    return 0;
}

PILF_INTERNAL int message_workflow_init(minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);

    EVENTS_BIND_NEW_LISTENER(time, ctx, MINION_TIME_STEP_EVENT,
        _handle_time_step_event);
    EVENTS_BIND_NEW_LISTENER(link, ctx, MINION_LINK_INMSG_EVENT,
        _handle_link_inmsg_event);
    EVENTS_BIND_NEW_LISTENER(engine, ctx, MINION_ENGINE_DONE_EVENT,
        _handle_engine_done_event);

    return 0;
}

PILF_INTERNAL int message_workflow_finalize(minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();
    
    ASSERT_NNULL(ctx);

    

    return 0;
}