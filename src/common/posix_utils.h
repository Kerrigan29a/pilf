#ifndef __PILF_MINION__SLEEP_H__
#define __PILF_MINION__SLEEP_H__



#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif



#ifdef WIN32
    #define pilf_sleep(seconds) Sleep(seconds * 1000)
#else
    #define pilf_sleep(seconds) sleep(seconds)
#endif



#ifdef WIN32
    #define pilf_getcwd(buffer, size) _getcwd(buffer, (int) size)
#else
    #define pilf_getcwd(buffer, size) getcwd(buffer, size)
#endif



#endif /* __PILF_MINION__SLEEP_H__ */