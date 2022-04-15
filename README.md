# Nuclear Engine

## Current To-dos 
  - W.I.P:
	- Improving PhysX Implementaion
	- Improving Scripting support (through AngelScript)
	- Improving Mesh Animation Implementaion
    - Improving README.md
	
  - TODO:
    - Graphical Improvement to the available Pipelines (ShadowMaps & ...)
	- Basic Editor support (Nuclear Editor).

# Getting started

Follow the instructions below to compile and run the engine from source.

## Windows

* Visual Studio 2022 or newer is required
* Clone repo & submodules
* Run **RunCmakeForDependencies.bat**
* Compile Assimp & Diligent & GLFW found in External/CmakeProjects
* Download and compile the remaining dependencies (PHYSX & FreeImage & FMOD)
* Run **CopyDependenciesBinaries.bat**
* Open `Nuclear Engine.sln` and set your solution configuration and platform
* Compile 


| Dependencies | Type |
| ------ | ------ |
| Assimp | Submodule & built with cmake |
| DiligentCore | Submodule & built with cmake |
| GLFW | Submodule & built with cmake |
| AngelScript | included |
| IMGUI | included |
| stb_truetype | included |
| entt | included |
| FMOD SDK | External 'Download it through their site' |
| PhysX 4.1 | submodule |
| Freeimage | submodule |