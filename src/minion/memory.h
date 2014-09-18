#ifndef __PILF_MINION__MEMORY_H__
#define __PILF_MINION__MEMORY_H__

#include "minion_internals.h"

#define _minion_free_and_burn(ctx, ptr) do { \
    _minion_free(ctx, ptr); \
    ptr = NULL; \
} while(0)


#define _minion_secure_free_and_burn(ctx, ptr, size)  do { \
    _minion_secure_free(ctx, ptr, size); \
    ptr = NULL; \
} while(0)

#ifdef UNIT_TESTING
#define MEMORY_DEBUG_INFO_ARGS_DECLARATION  , const char* file, const int line
#define MEMORY_DEBUG_INFO_ARGS              , file, line
#define MEMORY_DEBUG_INFO_VALUES            , __FILE__, __LINE__

#else
#define MEMORY_DEBUG_INFO_ARGS_DECLARATION
#define MEMORY_DEBUG_INFO_ARGS
#define MEMORY_DEBUG_INFO_VALUES
#endif

#ifdef UNIT_TESTING
#else
#endif

#define minion_malloc(ctx, size) \
    _minion_malloc(ctx, size MEMORY_DEBUG_INFO_VALUES)
#define minion_secure_malloc(ctx, size) \
    _minion_secure_malloc(ctx, size MEMORY_DEBUG_INFO_VALUES)
#define minion_free(ctx, ptr) \
    _minion_free_and_burn(ctx, ptr MEMORY_DEBUG_INFO_VALUES)
#define minion_secure_free(ctx, ptr, size) \
    _minion_secure_free_and_burn(ctx, ptr, size MEMORY_DEBUG_INFO_VALUES)
#define minion_strndup(ctx, str, size) \
    _minion_strndup(ctx, str, size MEMORY_DEBUG_INFO_VALUES)
#define minion_strdup(ctx, str) \
    _minion_strdup(ctx, str MEMORY_DEBUG_INFO_VALUES)



#endif /* __PILF_MINION__MEMORY_H__ */
