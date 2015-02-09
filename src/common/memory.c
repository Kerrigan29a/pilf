#include <stdlib.h>
#include <string.h>

#include "common_internals.h"


/*
 * http://www.dwheeler.com/secure-programs/Secure-Programs-HOWTO/protect-secrets.html
 */
PILF_INTERNAL void * pilf_memset(void *ptr, int c, size_t size)
{
    volatile char * tmp = ptr;

    while (size--) {
        *tmp++ = c;
    }
    return ptr;
}


PILF_INTERNAL void * _pilf_malloc(size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    void * ptr;

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

PILF_INTERNAL void * _pilf_secure_malloc(size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    void * ptr;

    ASSERT_TRUE(size > 0);

    ptr = _pilf_malloc(size MEMORY_DEBUG_INFO_ARGS);
    CHECK_NNULL_R(ptr, NULL);
    pilf_memset(ptr, 0x0, size);
    return ptr;
}

PILF_INTERNAL void _pilf_free(void * ptr
    MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    ASSERT_NNULL(ptr);

    // TODO: Search for custom free and use it
#ifdef UNIT_TESTING
    /* Cmockery2 function */
    _test_free(ptr MEMORY_DEBUG_INFO_ARGS);
#else
    free(ptr);
#endif
}

PILF_INTERNAL void _pilf_secure_free(void * ptr,
    size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    ASSERT_NNULL(ptr);
    ASSERT_TRUE(size > 0);

    pilf_memset(ptr, 0x0, size);
    _pilf_free(ptr MEMORY_DEBUG_INFO_ARGS);
}

PILF_INTERNAL char * _pilf_strndup(const char * str, size_t size MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    char * new_str;
    size_t amount;
    size_t str_len;

    ASSERT_STR(str);

    str_len = strlen(str);
    amount = size < str_len ? size + 1 : str_len + 1;

    new_str = (char *) _pilf_malloc(amount MEMORY_DEBUG_INFO_ARGS);
    CHECK_STR_R(new_str, NULL);
    memcpy(new_str, str, amount);
    new_str[amount - 1] = '\0';
    ASSERT_STR(new_str);
    ASSERT_TRUE(strlen(new_str) == amount - 1);
    return new_str;
}

PILF_INTERNAL char * _pilf_strdup(const char * str MEMORY_DEBUG_INFO_ARGS_DECLARATION)
{
    ASSERT_STR(str);

    return _pilf_strndup(str, strlen(str) MEMORY_DEBUG_INFO_ARGS);
}
