#ifndef __UBERNGINE_DEFS_H__
#define __UBERNGINE_DEFS_H__

#if defined(__APPLE__)
#include <TargetConditionals.h>
#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#define IOS_SYS
#else
#define MAC_SYS
#endif
#endif

enum PlatformType {UBE_GLFW, UBE_IOS};
//Integer types
#if defined(__APPLE__) || defined(linux) || defined(_WIN32)
#include <cstdint>
#endif
#endif
