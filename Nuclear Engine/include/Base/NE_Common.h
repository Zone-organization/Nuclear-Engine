#pragma once
#include <Base\NE_PrivateCommon.h>
#include <Base\Utilities\Logger.h>
#include <Base\BuildConfig.h>
#include <Base\NE_Exception.h>
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