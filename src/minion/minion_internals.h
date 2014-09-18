#ifndef __PILF_MINION__MINION_INTERNALS_H__
#define __PILF_MINION__MINION_INTERNALS_H__


/*******************************************************************************
 * Types of declarations
 ***/
#ifndef PILF_PRIVATE
#ifndef UNIT_TESTING
#define PILF_PRIVATE static
#else
#define PILF_PRIVATE
#endif /* UNIT_TESTING */
#endif
#ifndef PILF_PUBLIC
#define PILF_PUBLIC
#endif

// #ifdef _WIN32
// #define inline __inline
// #else
// #define inline inline
// #endif

/*******************************************************************************
 * Includes
 ***/

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
#include "sleep.h"
#include "time_events.h"
#include "utlist_utils.h"
#include "../common/check.h"

#include "public_decl.h"
#include "internal_decl.h"
#ifdef UNIT_TESTING
#include "private_decl.h"
#include <cmockery/cmockery_override.h>
#endif /* UNIT_TESTING */



/*******************************************************************************
 * Trace
 ***/

/* http://stackoverflow.com/a/2282433 */
#if defined ( WIN32 )
#define __func__ __FUNCTION__
#endif

#define TRACE_FUNCTION_CALL(ctx, fu, fi, li) trace(ctx, "%s (%s:%d)", fu, fi, li)

#endif /* __PILF_MINION__MINION_INTERNALS_H__ */