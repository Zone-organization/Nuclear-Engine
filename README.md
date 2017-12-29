# Nuclear Engine
![Alt text](http://media.indiedb.com/images/engines/1/1/764/NELOgo.png)
A Fast Cross-Platform Multi-Renderer 3D Game Engine using OpenGL 3.3+ , DirectX 11

## Features:-
- __Core__
  - Crossplatform ready
  - Modern Fast code using C++
  
- __ResourceManager__
  - Support for PNG, JPEG, BMP, DDS, GIF, HDR, PSD and many more image formats (using stb_image library).
  - Support for OBJ, FBX, Blend, 3DS, CAD, and many more 3D model formats (using Assimp library).

- __OneAPI__
  - Support for DirectX 11 "Feature Level 10.1" and OpenGL 3.3 Core Renderers.
  - Support for Point, Bilinear, Trilinear, Anisotropic "X2, X4, X8, X16", Filtering for Textures.
  - Support for HLSL Shaders which can be Cross Compiled to GLSL.
  - Fastest Design Possible through If statements in debug build and direct RenderAPI calls in release build
  
## Current Platforms
  - Windows 7+     

## Current Milestones
  - Finish the OneAPI:
    - Finish RenderTarget class (also known as Framebuffer in OpenGL).
  - Correctly Implement Model Loading with Assimp.
  - Implement Phong-Lighting.

## Roadmap
  - Support Android & Linux & UWP Platforms.
  - Support PhysX or Bullet Physics Library.
  - Support A Uber Shader instead of GLSL , HLSL.
  - Support Text Renderering With immediate mode GUI.
  - There are more but these are the most important...
  
  
### Building
Currently Visual Studio 2017 15.5.5 to build it and look at Deps section

### Architecture
Features:
  - All frequently called API functions and classes are 100% allocated on the Stack.
  - Design allows you to take advantage of its flexability and define your own renderers!
  - No Dynamic Inheritance or slow Function Pointers in any API call!
  
Summary:
Designed for Speed & Med-End Machines unlike other engine such as unreal engine that requires high end system
requirements or amazon lamburyard that takes whole your HDD Space.

Detailed Description:
It is designed to support multiple renderers such as OpenGL3 And DirectX11, where on some platforms as windows
you can choose one of them at runtime, which will cost you the smallest amount of performance since the engine
is build to avoid Heap Allocations and Bad Core design as dynamic inheritance (which uses slow function pointers
behind the scene), Most of the RenderAPI calls are usually designed in a way that lets you choose the renderer
in debug so you can check that your game works on all renderers and at build time the engine chooses the best
renderer for your platform to remove the entire API calling overhead. 


### Required Deps
  - Glew
  - Windows 32 API (SDK) (Which has DirectX & XAudio)

  
  