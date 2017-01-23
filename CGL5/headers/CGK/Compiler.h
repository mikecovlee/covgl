#ifndef CGL_COMPILER_H


#define CGL_COMPILER_H


#include "Basic.h"
#if defined(__ANDROID__) || defined(ANDROID)
#define CGL_KBLAYOUT_PHONE
#include "Unix.h"
#else
#if defined(__WIN32__) || defined(WIN32)
#define CGL_KBLAYOUT_PC
#include "Win32.h"
#else
#define CGL_KBLAYOUT_PC
#include "Unix.h"
#endif
#endif


#endif