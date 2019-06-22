![Nuclear Engine Logo](logo.png "Nuclear Engine Logo") 

# To-dos 
  - W.I.P:
    - Flexible Rendering pipeline and material system. [95%]
    - Restore Blinn Phong rendering pipeline Support.  [90%]
	- Test all the working engine features and list them [40%]

  - Planned:
    - Bloom effect.
    - PBR Renderer.
    - Basic Physics support with PhysX 4.
    - Restore Skybox support.
    - Restore Post Processing Effects support.

  - On Hold:
    - Full Audio Support with FMOD.		 [50%]

  - Pretty much far (Due to lack of knowledge how to achieve them or requiring full engine restructure):
    - Multithreading.
    - Android Support (Mobile devices in general).

# Features (Missing Few - Section W.I.P)

- __Core__
  - Cross platform ready.
  - Modern Fast code using C++17.
  - Fast Robust Entity Component System "ECS" using EntityX.
  - Cross-platform Application creation through GLFW.
  - Support for XML & INI Parsers.
  
- __Managers__  
  - AssetManager
    - Support for PNG, JPEG, BMP, DDS, GIF, HDR, PSD and many more image formats (using stb_image library).
    - Support for OBJ, FBX, Blend, 3DS, CAD, GLtf and many more 3D model formats (using Assimp library).
	- Support for OGG, FLAC, WAV, MP3 and many more audio formats (using FMOD library).
  - ShaderManager
    - Support for automated shader creation instead of Re-Writing them

- ___Audio___
  - Support for FMOD LowLevel API
  - Support for Preloading audio files to memory then playing them 			"Sound"
  - Support for streaming large audio files and playing them 				"Stream"  --Soon™

- __Graphics__
  - Support for Abstracted RenderAPI called Diligent that supports DirectX 11, DirectX 12, Vulkan backends.
  - Support for HLSL Shaders which can be Cross Compiled to GLSL or SPIR-V.
	 
### Building  (Section W.I.P)

Besure to update all submodules to be able to build it
Use Visual Studio 2019 to compile and build it.