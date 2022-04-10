#pragma once
#include <Core\NE_PrivateCommon.h>
#include <Core\Utilities\Logger.h>
#include <Core\BuildConfig.h>

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _DEBUG
#define DEBUG_TRUE_BOOL true
#define DEBUG_FALSE_BOOL false
#else
#define DEBUG_TRUE_BOOL false
#define DEBUG_FALSE_BOOL true
#endif