#ifndef __MINION_H__
#define __MINION_H__


#include <assert.h>
#include <stdlib.h>
#include <string.h>


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


/*******************************************************************************
 * Unit testing related configs
 ***/
#ifdef UNIT_TESTING
extern void mock_assert(const int result, const char* const expression,
    const char * const file, const int line);
#undef assert
#define assert(expression) \
    mock_assert(((expression) ? 1 : 0), #expression, __FILE__, __LINE__);
#endif /* UNIT_TESTING */


/*******************************************************************************
 * Includes
 ***/
#include "server.h"
#include "internal_decl.h"
#ifdef UNIT_TESTING
#include <uv.h>
#include "private_decl.h"
#endif /* UNIT_TESTING */


/*******************************************************************************
 * Utils
 ***/
#define XSTR(e) STR(e)
#define STR(e)  #e

#if defined(NDEBUG)
#define ASSERT_TRUE(e)      ((void)0)
#define ASSERT_FALSE(e)     ((void)0)
#define ASSERT_NOT_NULL(p)  ((void)0)
#define ASSERT_STR(s)       ((void)0)
#else
#define ASSERT_TRUE(e)      assert(e)
#define ASSERT_FALSE(e)     assert(!e)
#define ASSERT_NOT_NULL(p)  assert(p != NULL)
#define ASSERT_STR(s)       do {                                               \
    assert(s != NULL);                                                         \
    assert(strlen(s) > 0);                                                     \
} while (0)
#endif

#define CHECK_RETURN(e, r)          do { if(!(e)) return r; } while (0)
#define CHECK_ABORT(e)              do { if(!(e)) abort(); } while (0)
#define CHECK_NOT_NULL_RETURN(p, r) CHECK_RETURN(p != NULL, r)
#define CHECK_UNREACHABLE_ABORT()   do {                                       \
    printf("%s:%u Unreachable code reached.\n", __FILE__, __LINE__);           \
    abort();                                                                   \
} while (0)

#endif /* __MINION_H__ */