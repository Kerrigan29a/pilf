#ifndef __PILF_COMMON__COMMON_INTERNALS_H__
#define __PILF_COMMON__COMMON_INTERNALS_H__


/*******************************************************************************
 * Includes
 ***/

#include "platform.h"
#include "declarations.h"
#include "check.h"
#include "console_colors.h"
#include "posix_utils.h"

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

#endif /* __PILF_COMMON__COMMON_INTERNALS_H__ */