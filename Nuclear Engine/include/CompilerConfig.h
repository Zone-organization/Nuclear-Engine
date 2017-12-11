#pragma once

#ifdef _WIN32

#define PTE_SYSTEM_WINDOWS

#ifdef PTE_IMPLEMENTATION

#include <Windows.h>


extern int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main(__argc, __argv);
}

#endif

#endif