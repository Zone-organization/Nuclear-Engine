<p align="center">
  <img src="https://user-images.githubusercontent.com/11663821/206862862-6e1c396d-f782-43e8-bbc1-1bb7f8a38184.png">
</p>

## Current To-dos 
  - W.I.P:
  	- Serializing the scene & assets. (Current WIP)
	- Improving AssetManager
	- Multithreading Engine

	
  - On-Hold:
    - Text rendering (text tile-set creation finished)
	- Basic Editor support (Nuclear Editor).
	- PostProcessing effects should be processed per camera not only in the main camera.
	- Finish reflecting the C++ api to the C# scripting assembly (prob provide a build-time solution that builds a binding lib?)

  - TODO:
  	- CSM
	- SSAO
	- Motion blur
	- FIX BRDF generation in IBL Pipeline
	- Clean and document Samples.
	- GUI (basic widgets)

## Getting started

Follow the instructions below to compile and run the engine from source.

## Windows

* Visual Studio 2022 or newer is required
* Clone repo & submodules
* Run **RunCmakeForDependencies.bat**
* Compile Assimp & Diligent found in External/CmakeProjects
* Download and compile the remaining dependencies (PHYSX & FreeImage & Freetype & msdf-atlas-gen 'as static lib')
* Run **CopyDependenciesBinaries.bat**
* Open `Nuclear Engine.sln` and set your solution configuration and platform
* Compile 


| Dependencies | Type | Buildling |
| ------ | ------ | ------ |
| Assimp | Submodule | built with cmake |
| DiligentCore | Submodule | built with cmake |
| PhysX 4.1 | submodule | Built with their provided build-tools |
| Freeimage | submodule | Built with their provided build-tools |
| Freetype | submodule | Built with their provided build-tools |
| msdf-atlas-gen | submodule | Built with their provided build-tools |
| Mono | External | Put headers in External/mono |
| SDL2 | External | Put headers in External/SDL/include |
| spdlog | included | Built with the engine |
| IMGUI | included | Built with the engine |
| stb_truetype | included | Built with the engine |
| entt | included | Built with the engine |
| zpp::bits | included | Built with the engine |

Download external dependencies through their sites.
Put all their .lib in the bin folder.

## RoadMap
  - V0.01 Alpha:
    - Complete RenderSystem (50%) 
	- Complete AudioSystem Implementation. (?%)
	- Basic Implementaion of PhysX.
	- Complete Samples & their documentation.

## Preview

![Sample2](Sample2.gif "Sample2 Demo Logo") 
