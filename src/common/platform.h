#ifndef __PILF_COMMON__PLATFORM_H__
#define __PILF_COMMON__PLATFORM_H__

/*******************************************************************************
 * Guess platform
 ***/

/*
 * From: https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
 *
 * #ifdef _WIN32
 *    //define something for Windows (32-bit and 64-bit, this part is common)
 *    #ifdef _WIN64
 *       //define something for Windows (64-bit only)
 *    #endif
 * #elif __APPLE__
 *     #include "TargetConditionals.h"
 *     #if TARGET_IPHONE_SIMULATOR
 *          // iOS Simulator
 *     #elif TARGET_OS_IPHONE
 *         // iOS device
 *     #elif TARGET_OS_MAC
 *         // Other kinds of Mac OS
 *     #else
 *         // Unsupported platform
 *     #endif
 * #elif __linux
 *     // linux
 * #elif __unix // all unices not caught above
 *     // Unix
 * #elif __posix
 *     // POSIX
 * #endif
 */

#ifdef _WIN32
   #define PILF_WINDOWS
#elif __APPLE__
    #define PILF_DARWIN
#elif __linux
    #define PILF_LINUX
#else
    #error "Unknown platform"
#endif

/*******************************************************************************
 * Normalize platforms
 ***/

#if defined ( PILF_WINDOWS )
#define __func__ __FUNCTION__ /* http://stackoverflow.com/a/2282433 */
#define inline __inline
#endif

#endif /* __PILF_COMMON__PLATFORM_H__ */