#ifndef __PILF_COMMON__DECLARATIONS_H__
#define __PILF_COMMON__DECLARATIONS_H__

#ifndef TOSTR
#define TOSTR(x) _TOSTR(x)
#define _TOSTR(x)  #x
#endif
#define DO_PRAGMA(x) _Pragma(TOSTR(x))

#ifndef PILF_PRIVATE
  #ifndef UNIT_TESTING
    #define PILF_PRIVATE DO_PRAGMA(dgrep export PILF_PRIVATE __LINE__) static
  #else
    #define PILF_PRIVATE DO_PRAGMA(dgrep export PILF_PRIVATE __LINE__)
  #endif /* UNIT_TESTING */
#endif
#ifndef PILF_INTERNAL
  #define PILF_INTERNAL DO_PRAGMA(dgrep export PILF_INTERNAL __LINE__)
#endif
#ifndef PILF_PUBLIC
  #define PILF_PUBLIC DO_PRAGMA(dgrep export PILF_PUBLIC __LINE__)
#endif

#endif /* __PILF_COMMON__DECLARATIONS_H__ */
