#pragma once

#ifdef _WIN32

#define NUCLEAR_PLATFORM_WINDOWS32

#ifdef NUCLEAR_ENGINE_MAIN_IMPLEMENTATION

#include <Windows.h>

extern int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main(__argc, __argv);
}

#endif

#endif

#ifdef NE_USE_OPENGL3_3
#define NE_COMPILE_OPENGL3_3
#define NE_COMPILE_XSHADERCOMPILER
#endif

#ifdef NE_USE_DIRECTX11
#define NE_COMPILE_DIRECTX11
#define NE_COMPILE_D3DCOMPILER
#endif

#define NE_USE_RUNTIME_RENDERER

#ifdef NE_USE_RUNTIME_RENDERER

#ifdef NE_USE_OPENGL3_3
#error "Nuclear Engine: Cannot use seperate renderers with runtime renderer!"
#endif

#ifdef NE_USE_DIRECTX11
#error "Nuclear Engine: Cannot use seperate renderers with runtime renderer!"
#endif

#define NE_COMPILE_OPENGL3_3
#define NE_COMPILE_DIRECTX11
#define NE_COMPILE_D3DCOMPILER
#define NE_COMPILE_XSHADERCOMPILER

#ifndef _DEBUG
#warning "Nuclear Engine: Usage of runtime chosable renderer slows down performance!"
#endif

#else 

#endif