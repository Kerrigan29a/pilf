#ifndef __PILF_MINION__SLEEP_H__
#define __PILF_MINION__SLEEP_H__


#ifdef WIN32
#include <windows.h>

#define minion_sleep(seconds) Sleep(seconds * 1000)

#else
#include <unistd.h>

#define minion_sleep(seconds) sleep(seconds)

#endif

#endif /* __PILF_MINION__SLEEP_H__ */