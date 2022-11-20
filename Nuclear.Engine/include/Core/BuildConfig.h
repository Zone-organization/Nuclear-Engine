#pragma once

#ifdef _WIN32

#ifdef _WIN64
#define NUCLEAR_PLATFORM_WINDOWS_PC_64BIT
#else
#define NUCLEAR_PLATFORM_WINDOWS_PC_32BIT
#endif

#define NUCLEAR_PLATFORM_WINDOWS_PC
#endif

#ifdef _DEBUG
#define NUCLEAR_CONFIG_DEBUG
#else
#define NUCLEAR_CONFIG_RELEASE
#endif
/*
Building Configs for the whole engine goes here.

Building options:
-NE_USE_CORE_OPENGL
	-Builds whole engine in OpenGL Render API (Defines NE_COMPILE_CORE_OPENGL)
	-Also uses XShaderCompiler to cross-compile HLSL to GLSL (Defines NE_COMPILE_XSHADERCOMPILER)
-NE_USE_DIRECTX11
	-Builds whole engine in DirectX Render API (Defines NE_COMPILE_DIRECTX11)
	-Also uses DirectX HLSL Compiler to compile HLSL to DXBC (Defines NE_COMPILE_D3DCOMPILER)
-NE_USE_RUNTIME_RENDER_API
	-Builds the engine to use both DirectX11 and OpenGL 3 render APIs, (May have impact on performance so not recommended for release)
-NE_USE_RENDER_API_DEBUG_LAYER
	-Enable the Render-API debug layer to trace and detect errors
-NE_USE_PHYSICS3DENGINE
	-Enabled Nvidia PhysX Engine integration (not suitable for 2D).
*/

//#include <PhysX\PhysXincluder.h>
//#if !defined(PHYSX_NOT_INCLUDED)
//#define NE_USE_PHYSICS3DENGINE
//#endif

#if defined (NE_USE_PHYSICS3DENGINE) && defined(PHYSX_NOT_INCLUDED)
#error PhysX isnot available
#endif

#define NE_COMPILE_D3DCOMPILER
#define NE_COMPILE_XSHADERCOMPILER

