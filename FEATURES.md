# Features
All features (without Soon™) listed here are implemented.
Features with Buggy™ means it need more testing or work.

- __Core__
  - Cross platform ready.
  - Modern Fast code using C++14.
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
  - Support for Abstracted RenderAPI called "OneAPI" having implementations in DirectX 11 "Feature Level 10.1" and OpenGL 3.3 Core RenderAPIs.
  - Support for HLSL Shaders which can be Cross Compiled to GLSL.
  - Support for Separate Shaders & Seperate Samplers, Textures.
  - Support for Customized Dear-ImGui as a Default GUI Library.
  - ___RenderSystem___
    - Support for BlinnPhong Lighting.
    - Support for Normal Mapping.	--Buggy™
    - Support for PostProcessing Effects:
	  - Gamma Correction
	  - HDR 'Reinhard tone mapping'
	  
- __OneAPI__
  - Context
  - ConstantBuffer
  - VertexBuffer
  - IndexBuffer
  - InputLayout
  - Texture
  - Sampler
  - RenderTarget
  - BlendState
  - RasterizerState
  - DepthStencilState
  - ShaderCompiler
  - VertexShader
  - PixelShader
