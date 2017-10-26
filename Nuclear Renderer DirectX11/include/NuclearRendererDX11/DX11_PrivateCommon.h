#pragma once

#ifdef NUCLEARRENDERERDIRECTX11_EXPORTS
#define NRDX11API __declspec(dllexport)
#else
#define NRDX11API __declspec(dllimport)
#endif