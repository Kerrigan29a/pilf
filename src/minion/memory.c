#include <stdlib.h>
#include <string.h>

#include "minion_internals.h"


#if MINION_TRACE_MEMORY
#define MINION_TRACE_FUNCTION() TRACE_FUNCTION_CALL(ctx, __func__, __FILE__, __LINE__)
#else
#define MINION_TRACE_FUNCTION() ((void)0)
#endif


/*
 * http://www.dwheeler.com/secure-programs/Secure-Programs-HOWTO/protect-secrets.html
 */
PILF_INTERNAL void * minion_memset(minion_context_t * const ctx, void *ptr,
    int c, size_t size)
{
    volatile char * tmp = ptr;

    MINION_TRACE_FUNCTION();

    while (size--) {
        *tmp++ = c;
    }
    return ptr;
}


PILF_INTERNAL void * _minion_malloc(minion_context_t * const ctx, size_t size
    MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    void * ptr;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_TRUE(size > 0);

    // TODO: Search for custom malloc and use it
#ifdef UNIT_TESTING
    /* Cmockery2 function */
    ptr = _test_malloc(size MEMORY_DEBUG_INFO_ARGS);
#else
    ptr = malloc(size);
#endif
    ASSERT_NNULL(ptr);
    return ptr;
}

PILF_INTERNAL void * _minion_secure_malloc(minion_context_t * const ctx,
    size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    void * ptr;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_TRUE(size > 0);

    ptr = _minion_malloc(ctx, size MEMORY_DEBUG_INFO_ARGS);
    CHECK_NNULL_R(ptr, NULL);
    minion_memset(ctx, ptr, 0x0, size);
    return ptr;
}

PILF_INTERNAL void _minion_free(minion_context_t * const ctx, void * ptr
    MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(ptr);

    // TODO: Search for custom free and use it
#ifdef UNIT_TESTING
    /* Cmockery2 function */
    _test_free(ptr MEMORY_DEBUG_INFO_ARGS);
#else
    free(ptr);
#endif
}

PILF_INTERNAL void _minion_secure_free(minion_context_t * const ctx, void * ptr,
    size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_NNULL(ptr);
    ASSERT_TRUE(size > 0);

    minion_memset(ctx, ptr, 0x0, size);
    _minion_free(ctx, ptr MEMORY_DEBUG_INFO_ARGS);
}

PILF_INTERNAL char * _minion_strndup(minion_context_t * const ctx,
    const char * str, size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    char * new_str;
    size_t len;

    MINION_TRACE_FUNCTION();

    ASSERT_NNULL(ctx);
    ASSERT_STR(str);

    len = strlen(str);
    if (size < len) {
        len = size;
    }

    new_str = (char *) _minion_malloc(ctx, (size + 1) MEMORY_DEBUG_INFO_ARGS);
    CHECK_STR_R(new_str, NULL);
    new_str[size] = '\0';
    memcpy(new_str, str, size);
    return new_str;
}

PILF_INTERNAL char * _minion_strdup(minion_context_t * const ctx,
    const char * str MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    MINION_TRACE_FUNCTION();
    
    ASSERT_NNULL(ctx);
    ASSERT_STR(str);

    return _minion_strndup(ctx, str, strlen(str) MEMORY_DEBUG_INFO_ARGS);
}
