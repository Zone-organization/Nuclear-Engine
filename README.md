# Nuclear Engine

![Sample2](Sample2.gif "Sample2 Demo Logo") 

## Current To-dos 
  - W.I.P:
	- C# scripting through mono
	- AssetLoading Multithreading
	
  - On-Hold:
    - Text rendering (text tile-set creation finished)
	- Basic Editor support (Nuclear Editor).

  - TODO:
  	- CSM
	- SSAO
	- Motion blur
    - Finish Audio implementation
	- FIX BRDF generation in IBL Pipeline
	- Clean and document Samples.
	- GUI (basic widgets)
	
## Getting started

Follow the instructions below to compile and run the engine from source.

## Windows

* Visual Studio 2022 or newer is required
* Clone repo & submodules
* Run **RunCmakeForDependencies.bat**
* Compile Assimp & Diligent & GLFW found in External/CmakeProjects
* Download and compile the remaining dependencies (PHYSX & FreeImage & FMOD & Freetype & msdf-atlas-gen 'as static lib')
* Run **CopyDependenciesBinaries.bat**
* Open `Nuclear Engine.sln` and set your solution configuration and platform
* Compile 


| Dependencies | Type |
| ------ | ------ |
| Assimp | Submodule & built with cmake |
| DiligentCore | Submodule & built with cmake |
| PhysX 4.1 | submodule |
| Freeimage | submodule |
| Freetype | submodule |
| msdf-atlas-gen | submodule |
| spdlog | included |
| IMGUI | included |
| stb_truetype | included |
| entt | included |
| cereal | included |
| Mono | External 'Download it through their site' |
| FMOD SDK | External 'Download it through their site' |
| SDL2 | External 'Download it through their site' |


## RoadMap
  - V0.01 Alpha:
    - Complete RenderSystem (50%) 
	- Complete AudioSystem Implementation. (?%)
	- Basic Implementaion of PhysX.
	- Complete Samples & their documentation.