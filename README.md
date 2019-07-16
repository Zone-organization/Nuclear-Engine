![Nuclear Engine Logo](logo.png "Nuclear Engine Logo") 
* [Engine features](https://github.com/Zone-organization/Nuclear-Engine/blob/master/FEATURES.md) - A list of all working features.

# To-dos 
  - W.I.P:
	- Port All Components to ECS (80%)
	- Basic Editor support (Nuclear Editor).

  - Planned:
    - Restore & Test render to texture feature (Render Target)
    - Bloom effect.
    - C# Scripting through .Net Core
	- Mesh Animations

  - Pretty much far (Due to lack of knowledge how to achieve them or requiring full engine restructure):
    - Multithreading.
    - Android Support (Mobile devices in general).

## Known Bugs
  - Rendering any model with mirrored UVs seems to be broken.

### Building
The project has three building configuration:
Debug: Which is used in internal engine development.
Checked: Use it if you are developing a game.
Release: Provides maximum performance, use it if you are publishing your game.

Some Dependencies such as FMOD & PhysX & FreeImage needs to be downloaded and built manually.

- __Requirements__
  - Visual Studio 2019
  - CMake

- __Dependencies__
  - Assimp
  - DiligentCore
  - FMOD SDK
  - PhysX 4.1
  - Freeimage
  - GLFW
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  