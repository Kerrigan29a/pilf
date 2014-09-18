    #include <stdlib.h>

#include "minion_internals.h"

#if MINION_TRACE_MINION
#define MINION_TRACE_FUNCTION() TRACE_FUNCTION_CALL(ctx, __func__, __FILE__, __LINE__)
#else
#define MINION_TRACE_FUNCTION() ((void)0)
#endif

PILF_PUBLIC void minion_start(minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();

    CHECK_NNULL_A(ctx);

    CHECK_ZERO_A(events_init(ctx));
    CHECK_ZERO_A(link_init(ctx));
    CHECK_ZERO_A(engine_init(ctx));
    CHECK_ZERO_A(message_workflow_init(ctx));

    events_dispatch(ctx);
}

PILF_PUBLIC void minion_stop(minion_context_t * const ctx)
{
    MINION_TRACE_FUNCTION();
    
    CHECK_NNULL_A(ctx);

    CHECK_ZERO_A(message_workflow_finalize(ctx));
    CHECK_ZERO_A(events_finalize(ctx));
    CHECK_ZERO_A(link_finalize(ctx));
    CHECK_ZERO_A(engine_finalize(ctx));
}