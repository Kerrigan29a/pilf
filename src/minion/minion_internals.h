#ifndef __PILF_MINION__MINION_INTERNALS_H__
#define __PILF_MINION__MINION_INTERNALS_H__


/*******************************************************************************
 * Includes
 ***/

/* Define log functions to use in ../common/check.h */
#define LOG_ERROR(...)      error(ctx, __VA_ARGS__)
#define LOG_CRITICAL(...)   critical(ctx, __VA_ARGS__)


#include "../common/common_internals.h"
#include "minion_config.h"
#include "context.h"
#include "engine.h"
#include "engine_events.h"
#include "events.h"
#include "link.h"
#include "link_events.h"
#include "log.h"
#include "memory.h"
#include "message_workflow.h"
#include "time_events.h"
#include "utlist_utils.h"

#ifndef __DGREP__
    #include "public_decl.h"
    #include "internal_decl.h"
#endif  /* __DGREP__ */
#ifdef UNIT_TESTING
    #ifndef __DGREP__
        #include "private_decl.h"
    #endif  /* __DGREP__ */
    #include <cmockery/cmockery_override.h>
#endif /* UNIT_TESTING */



/*******************************************************************************
 * Trace
 ***/

#define TRACE_FUNCTION_CALL(ctx, fu, fi, li) trace(ctx, "%s (%s:%d)", fu, fi, li)

#endif /* __PILF_MINION__MINION_INTERNALS_H__ */