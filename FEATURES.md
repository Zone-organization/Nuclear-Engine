# Features

# Note:
# These features are now missing & Incomplete due to replacing OneAPI with Diligent.
# it is planned to update this list and restore missing features later.

All features (without Soon™) listed here are implemented.
Features with Buggy™ means it need more testing or work.

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
  - Support for Abstracted RenderAPI called Diligent that supports DirectX 11, DirectX 12, Vulkan and OpenGL 4 backends.
  - Support for HLSL Shaders which can be Cross Compiled to GLSL or SPIR-V.
  - Support for Customized Dear-ImGui as a Default GUI Library.
  - ___RenderSystem___
    - Support for BlinnPhong Lighting.
    - Support for Normal Mapping.	--Buggy™
    - Support for PostProcessing Effects:
	  - Gamma Correction
	  - HDR 'Reinhard tone mapping'