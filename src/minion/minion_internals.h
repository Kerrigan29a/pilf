#ifndef __PILF_MINION__MINION_INTERNALS_H__
#define __PILF_MINION__MINION_INTERNALS_H__


/*******************************************************************************
 * Includes
 ***/

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
#include "sleep.h"
#include "time_events.h"
#include "utlist_utils.h"

#include "public_decl.h"
#include "internal_decl.h"
#ifdef UNIT_TESTING
#include "private_decl.h"
#include <cmockery/cmockery_override.h>
#endif /* UNIT_TESTING */



/*******************************************************************************
 * Trace
 ***/

#define TRACE_FUNCTION_CALL(ctx, fu, fi, li) trace(ctx, "%s (%s:%d)", fu, fi, li)

#endif /* __PILF_MINION__MINION_INTERNALS_H__ */