# Nuclear Engine
![Alt text](http://media.indiedb.com/images/engines/1/1/764/NELOgo.png)
A Fast Cross-Platform Multi-Renderer 3D Game Engine using OpenGL 3.3+ , DirectX 11

# Preview
![Alt text](http://media.indiedb.com/cache/images/engines/1/1/764/thumb_620x2000/Untitled.1.png)
![Alt text](http://media.indiedb.com/cache/images/engines/1/1/764/thumb_620x2000/Blending_Demo.png)

## Requirements
- __OpenGL__
  - OpenGL 3.3 Core capable GPU with the required extensions (all became core in later versions):
    - GL_ARB_shading_language_420pack
	- GL_ARB_separate_shader_objects

- __DirectX__
  - DirectX 10.1 capable GPU.
  - DirectX 11 runtime dlls.

## Features:-
- __Core__
  - Cross platform ready
  - Modern Fast code using C++
  
- __ResourceManager__
  - Support for PNG, JPEG, BMP, DDS, GIF, HDR, PSD and many more image formats (using stb_image library).
  - Support for OBJ, FBX, Blend, 3DS, CAD, GLtf and many more 3D model formats (using Assimp library).

- __OneAPI__
  - Support for DirectX 11 "Feature Level 10.1" and OpenGL 3.3 Core RenderAPIs.
  - Support for Point, Bilinear, Tri-linear, Anisotropic "X2, X4, X8, X16", Filtering for Textures.
  - Support for HLSL Shader which can be Cross Compiled to GLSL.
  - Support for Separate Shader or even faster Unified Shader.
  - Fastest Design Possible through If statements in debug build and direct RenderAPI calls in release build
  
- __Renderer__
  - Support for Hybrid Lighting system which allows you to implement more lighting techniques.
  - Support for Phong-Shading.
  - Support for IMGUI.
  
## Current Platforms
  - Windows 7+     

## Current Milestones
  - Improve the OneAPI:
    - Implement RenderTarget classes.

## Road-map
  - Support Android & Linux & UWP Platforms.
  - Support PhysX or Bullet Physics Library.
  - Support A Uber Shader instead of GLSL , HLSL.
  - Support Text Rendering With immediate mode GUI.
  - There are more but these are the most important...
  
  
### Building
Currently I am using Visual Studio 2017 15.5.5 to build it.
For Dependencies please see https://github.com/Zone-organization/Nuclear-Deps

### Architecture
Features:
  - All frequently called API functions and classes are 100% allocated on the Stack.
  - Design allows you to take advantage of its flexibility and define your own RenderAPIs!
  - No Dynamic Inheritance or slow Function Pointers in any API call!
  
Summary:
Designed for Speed & Med-End Machines unlike other engine such as unreal engine that requires high end system
requirements or amazon lumberyard that takes whole your HDD Space.

Detailed Description:
It is designed to support multiple RenderAPIs such as OpenGL3 And DirectX11, where on some platforms as windows
you can choose one of them at runtime, which will cost you the smallest amount of performance since the engine
is build to avoid Heap Allocations and Bad Core design as dynamic inheritance (which uses slow function pointers
behind the scene), Most of the RenderAPI calls are usually designed in a way that lets you choose the renderer
in debug so you can check that your game works on all RenderAPIs and at build time the engine chooses the best
renderer for your platform to remove the entire API calling overhead. 


Tutorials & references used (thank y'all!):
  - https://learnopengl.com/Introduction
  - http://www.d3dcoder.net/
  - http://www.mbsoftworks.sk/index.php?page=tutorials&series=1
  - https://stackoverflow.com/
  
  
