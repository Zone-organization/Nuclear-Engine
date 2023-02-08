<p align="center">
  <img src="https://user-images.githubusercontent.com/11663821/206862862-6e1c396d-f782-43e8-bbc1-1bb7f8a38184.png">
</p>
<h3 align="center">W.I.P Cross-platform 3D Game Engine written in C++20 with C# Scripting</h3>
<p align="center">
  <a href="#Building">Getting Started</a> |
  <a href="#Features">Features</a> |
  <a href="#Dependencies">Dependencies</a> |
  <a href="#RoadMap">RoadMap</a>

## Preview
For more examples, please see [Samples](https://github.com/Zone-organization/Nuclear-Engine/tree/master/Samples).
![Sample2](Sample2.gif "Sample2 Demo Logo") 

## Features
List is W.I.P (There are lots of features not written here)

<details>
	<summary><b>Engine</b></summary>
	
	- Entity Component System


</details>
<details>
	<summary><b>Assets</b></summary>

	- W.I.P

</details>
<details>
	<summary><b>Graphics</b></summary>

	- Dynamic light Comoponent (Directional & Point & Spot)
	


</details>
<details>
	<summary><b>Audio</b></summary>

	- Uses OpenAL or XAudio2 as a low-level backends

</details>
<details>
	<summary><b>Physics</b></summary>

	- PhysX 4.1 integration

</details>
<details>
	<summary><b>Scripting</b></summary>

	- C# Scripting through Mono


</details>


## Building

Follow the instructions below to compile and run the engine from source.
If you wish to build with another compiler/platform, please note that:
  - The engine requires a C++20 compiler and currently only supports x64 platforms (support for x32 is planned)
  - The engine doesn't contain platform-specific code and is written with cross-platform portability in mind, so porting to other platforms should be easy.

## Windows

* Visual Studio 2022 or newer is required if you wish to build the solution directly
* Clone repo & submodules
* Run **RunCmakeForDependencies.bat**
* Compile Assimp & Diligent & OpenAL-soft found in External/CmakeProjects
* Download and compile the remaining dependencies (PhysX & FreeImage & Freetype & msdf-atlas-gen 'as static lib')
* Run **CopyDependenciesBinaries.bat**
* Open `Nuclear Engine.sln` and set your solution configuration and platform
* Compile 

## Other Platforms
  - Planned platforms: 
    - Desktop: Linux - MacOS
    - Mobile: Android - IOS
	
Consoles are planned but not a priority now...


## Dependencies
| Dependencies | Type | Buildling |
| ------ | ------ | ------ |
| Assimp | Submodule | built with cmake |
| DiligentCore | Submodule | built with cmake |
| OpenAL-soft | Submodule | built with cmake |
| PhysX 4.1 | submodule | Built with their provided build-tools |
| Freeimage | submodule | Built with their provided build-tools |
| Freetype | submodule | Built with their provided build-tools |
| msdf-atlas-gen | submodule | Built with their provided build-tools |
| Mono | External | Put headers in External/mono |
| SDL2 | External | Put headers in External/SDL/include |
| libsndfile | External | Put the .lib in your Build Output directory |
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
	- Complete AudioSystem Implementation. (65%)
	- Basic Implementaion of PhysX.
	- Complete Samples & their documentation.

## Current To-dos 
  - W.I.P:
  	- AudioSystem Implementation using OpenAL-Soft/XAudio2. (70%)
  	- Serializing the scene & assets.
	- Use CMake instead of Visual Studio.
	
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
	
	
## Contributing

* Contributions are welcome, just open a pull request or an issue. You can also join us in GitHub Discussions.