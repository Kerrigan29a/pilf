#ifndef __PILF_COMMON__CHECK_H__
#define __PILF_COMMON__CHECK_H__


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*******************************************************************************
 * Utils
 ***/
#define TOSTR(e) _TOSTR(e)
#define _TOSTR(e)  #e

#define IS_NULL(p)          ((p) == NULL)
#define IS_NNULL(p)         ((p) != NULL)

#define IS_STR(s)           (IS_NNULL(s) && (strlen(s) > 0))
#define IS_STREQ(s1, s2)    (IS_NNULL(s1) && IS_NNULL(s2) && (strcmp(s1, s2)==0))
#define IS_STRNEQ(s1, s2)   (IS_NNULL(s1) && IS_NNULL(s2) && (strcmp(s1, s2)!=0))

#define IS_BITS_EQ(x, y)    ((x) == (y))
#define IS_BITS_IN(x, y)    (((x) & (y)) == (x))

#define IS_UNREACHABLE()    printf("%s:%u Unreachable code reached\n", __FILE__, __LINE__)

/*******************************************************************************
 * Asserts
 ***/
#define ASSERT_TRUE(e)          assert(e)
#define ASSERT_FALSE(e)         assert(!(e))

#define ASSERT_ZERO(e)          assert(!(e))

#define ASSERT_NULL(p)          ASSERT_TRUE(IS_NULL(p))
#define ASSERT_NNULL(p)         ASSERT_TRUE(IS_NNULL(p))

#define ASSERT_STR(s)           ASSERT_TRUE(IS_STR(s))
#define ASSERT_STREQ(s1, s2)    ASSERT_TRUE(IS_STREQ(s1, s2))
#define ASSERT_STRNEQ(s1, s2)   ASSERT_TRUE(IS_STRNEQ(s1, s2))

#define ASSERT_BITS_EQ(x, y)    ASSERT_TRUE(IS_BITS_EQ(x, y))
#define ASSERT_BITS_IN(x, y)    ASSERT_TRUE(IS_BITS_IN(x, y))


#ifdef NDEBUG
#define ASSERT_EXEC(x)          if (0)
#define ASSERT_UNREACHABLE()    ((void)0)
#else /* NDEBUG */
/* http://stackoverflow.com/a/10959486 */
#define ASSERT_EXEC(x)          for ( ; !(x) ; assert(x) )
#define ASSERT_UNREACHABLE()    do{ IS_UNREACHABLE(); ASSERT_TRUE(0); }while (0)
#endif /* NDEBUG */

/*******************************************************************************
 * Check
 ***/
#define CHECK_R(e, r)               do{ if(!(e)) return (r); }while (0)
#define CHECK_A(e)                  do{ if(!(e)) abort(); }while (0)
#define CHECK_G(e, l)               do{ if(!(e)){ goto l; }}while (0)

#define CHECK_TRUE_R(e, r)          CHECK_R(e, r)
#define CHECK_FALSE_R(e, r)         CHECK_R(!(e), r)

#define CHECK_ZERO_R(e, r)          CHECK_R(!(e), r)

#define CHECK_NULL_R(p, r)          CHECK_R(IS_NULL(p), r)
#define CHECK_NNULL_R(p, r)         CHECK_R(IS_NNULL(p), r)

#define CHECK_STR_R(s, r)           CHECK_R(IS_STR(s), r)
#define CHECK_STREQ_R(s1, s2, r)    CHECK_R(IS_STREQ(s1, s2), r)
#define CHECK_STRNEQ_R(s1, s2, r)   CHECK_R(IS_STRNEQ(s1, s2), r)

#define CHECK_BITS_EQ_R(x, y, r)    CHECK_R(IS_BITS_EQ(x, y), r)
#define CHECK_BITS_IN_R(x, y, r)    CHECK_R(IS_BITS_IN(x, y), r)


#define CHECK_TRUE_A(e)             CHECK_A(e)
#define CHECK_FALSE_A(e)            CHECK_A(!(e))

#define CHECK_ZERO_A(e)             CHECK_A(!(e))

#define CHECK_NULL_A(p)             CHECK_A(IS_NULL(p))
#define CHECK_NNULL_A(p)            CHECK_A(IS_NNULL(p))

#define CHECK_STR_A(s)              CHECK_A(IS_STR(s))
#define CHECK_STREQ_A(s1, s2)       CHECK_A(IS_STREQ(s1, s2))
#define CHECK_STRNEQ_A(s1, s2)      CHECK_A(IS_STRNEQ(s1, s2))

#define CHECK_BITS_EQ_A(x, y)    CHECK_A(IS_BITS_EQ(x, y))
#define CHECK_BITS_IN_A(x, y)    CHECK_A(IS_BITS_IN(x, y))


#define CHECK_TRUE_G(e, l)          CHECK_G(e, l)
#define CHECK_FALSE_G(e, l)         CHECK_G(!(e), l)

#define CHECK_ZERO_G(e, l)          CHECK_G(!(e), l)

#define CHECK_NULL_G(p, l)          CHECK_G(IS_NULL(p), l)
#define CHECK_NNULL_G(p, l)         CHECK_G(IS_NNULL(p), l)

#define CHECK_STR_G(s, l)           CHECK_G(IS_STR(s), l)
#define CHECK_STREQ_G(s1, s2, l)    CHECK_G(IS_STREQ(s1, s2), l)
#define CHECK_STRNEQ_G(s1, s2, l)   CHECK_G(IS_STRNEQ(s1, s2), l)

#define CHECK_BITS_EQ_G(x, y, l)    CHECK_G(IS_BITS_EQ(x, y), l)
#define CHECK_BITS_IN_G(x, y, l)    CHECK_G(IS_BITS_IN(x, y), l)


#define CHECK_UNREACHABLE()         do{ IS_UNREACHABLE(); abort(); }while (0)

#endif /* __PILF_COMMON__CHECK_H__ */