![Nuclear Engine Logo](logo.png "Nuclear Engine Logo") 

A Fast Cross-Platform Multi-Renderer 3D Game Engine using OpenGL 3.3+ , DirectX 11

# Large restructure is undergoing...
[![Build status](https://ci.appveyor.com/api/projects/status/k7lo2s60aa0gmld2?svg=true)](https://ci.appveyor.com/project/Zone-organization/nuclear-engine) 

# Features
* [Engine features](https://github.com/Zone-organization/Nuclear-Engine/blob/master/FEATURES.md) - A list of all working features.
* [Roadmap](https://github.com/Zone-organization/Nuclear-Engine/blob/master/ROADMAP.md) - A list of planned features. 

## Requirements
- __OpenGL__
  - OpenGL 3.3 Core capable GPU with the required extensions (all became core in later versions):
    - Required for Separate Shaders:	
	  - GL_ARB_shading_language_420pack.
	  - GL_ARB_separate_shader_objects.

- __DirectX__
  - DirectX 10.1 capable GPU.
  - DirectX 11 runtime DLLS.

## Current Platforms
  - Windows 7+

### Building
Please See NuclearDeps Repo for external dependencies.
Currently I am using Visual Studio 2017 to build it.

Included ThirdParty Deps:
  - Google CRC32c
  - SFML (as Platform Abstraction Layer "PAL" only)
  - STB_image