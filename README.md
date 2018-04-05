# Nuclear Engine
![Alt text](http://media.indiedb.com/images/engines/1/1/764/NELOgo.png)
A Fast Cross-Platform Multi-Renderer 3D Game Engine using OpenGL 3.3+ , DirectX 11
### Build Status
[![Build status](https://ci.appveyor.com/api/projects/status/k7lo2s60aa0gmld2?svg=true)](https://ci.appveyor.com/project/Zone-organization/nuclear-engine)

# Preview
![Alt text](http://media.indiedb.com/cache/images/engines/1/1/764/thumb_620x2000/Untitled.1.png)
![Alt text](http://media.indiedb.com/cache/images/engines/1/1/764/thumb_620x2000/cryteksponza_photo.png)
![Alt text](http://media.indiedb.com/cache/images/engines/1/1/764/thumb_620x2000/Blending_Demo.png)

## Requirements
- __OpenGL__
  - OpenGL 3.3 Core capable GPU with the required extensions (all became core in later versions):
    - Required for Separate Shaders:	
	  - GL_ARB_shading_language_420pack
	  - GL_ARB_separate_shader_objects

- __DirectX__
  - DirectX 10.1 capable GPU.
  - DirectX 11 runtime DLLS.

## Features:
- __Core__
  - Cross platform ready
  - Modern Fast code using C++
  - Fast Robust Entity Component System "ECS" using EntityX.
  - Cross-platform Application creation through GLFW.
  - All of error prone calls (as Resource creation or loading) are being logged.
  - Support for XML & INI Parsers.
  
- __AssetManager__
  - Support for PNG, JPEG, BMP, DDS, GIF, HDR, PSD and many more image formats (using stb_image library).
  - Support for OBJ, FBX, Blend, 3DS, CAD, GLtf and many more 3D model formats (using Assimp library).
  - Multi-Thread loading (Soon™)
  
- __ShaderManager__
  - Support for automated shader creation instead of Re-Writing them
  
- __Audio__
  - Support for FMOD.

- __Graphics__
  - Support for DirectX 11 "Feature Level 10.1" and OpenGL 3.3 Core RenderAPIs.
  - Support for Point, Bilinear, Tri-linear, Anisotropic "X2, X4, X8, X16", Filtering for Textures.
  - Support for HLSL Shaders which can be Cross Compiled to GLSL.
  - Support for Separate Shaders (Vertex & Pixel).
  - Support for Separate Samplers and Textures.
  - Support for Customized Dear-ImGui as a Default GUI Library.
  
  - ___RenderSystem___
    - Support for BlinnPhong Lighting.
    - Support for Normal Mapping.
    - Support for PostProcessing Effects:
	  - Gamma Correction
	  - HDR 'Reinhard tone mapping'
  
## Current Platforms
  - Windows 7+     

## Currently TODO
  - Implement PhysXEngine (Nvidia Physics)
  - Multi-thread crucial sub-systems.
  - Implement WYSIWYG Editor.
  - Implement Class based GUI from ImGui.
  
### Building
Please See NuclearDeps Repo for dependencies.
Currently I am using Visual Studio 2017 to build it.