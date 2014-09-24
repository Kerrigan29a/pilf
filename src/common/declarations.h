#ifndef __PILF_COMMON__DECLARATIONS_H__
#define __PILF_COMMON__DECLARATIONS_H__

#ifndef PILF_PRIVATE
  #ifndef UNIT_TESTING
    #define PILF_PRIVATE static
  #else
    #define PILF_PRIVATE
  #endif /* UNIT_TESTING */
#endif
#ifndef PILF_INTERNAL
  #define PILF_INTERNAL
#endif
#ifndef PILF_PUBLIC
  #define PILF_PUBLIC
#endif

#endif /* __PILF_COMMON__DECLARATIONS_H__ */
