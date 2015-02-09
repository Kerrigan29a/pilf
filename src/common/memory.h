#ifndef __PILF_COMMON__MEMORY_H__
#define __PILF_COMMON__MEMORY_H__



#ifdef UNIT_TESTING
    #define MEMORY_DEBUG_INFO_ARGS_DECLARATION  ,const char* file,const int line
    #define MEMORY_DEBUG_INFO_ARGS              ,file,line
    #define MEMORY_DEBUG_INFO_VALUES            ,__FILE__,__LINE__
#else
    #define MEMORY_DEBUG_INFO_ARGS_DECLARATION
    #define MEMORY_DEBUG_INFO_ARGS
    #define MEMORY_DEBUG_INFO_VALUES
#endif



#define pilf_malloc(size) \
    _pilf_malloc(size MEMORY_DEBUG_INFO_VALUES)
#define pilf_secure_malloc(size) \
    _pilf_secure_malloc(size MEMORY_DEBUG_INFO_VALUES)
#define pilf_free(ptr) do { \
    _pilf_free(ptr MEMORY_DEBUG_INFO_VALUES); \
    ptr = NULL; \
} while(0)
#define pilf_secure_free(ptr, size) do { \
    _pilf_secure_free(ptr, size MEMORY_DEBUG_INFO_VALUES); \
    ptr = NULL; \
} while(0)
#define pilf_strndup(str, size) \
    _pilf_strndup(str, size MEMORY_DEBUG_INFO_VALUES)
#define pilf_strdup(str) \
    _pilf_strdup(str MEMORY_DEBUG_INFO_VALUES)



#endif /* __PILF_COMMON__MEMORY_H__ */
