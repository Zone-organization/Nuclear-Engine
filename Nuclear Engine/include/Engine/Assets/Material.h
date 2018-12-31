#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets\Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>
#include <map>

namespace NuclearEngine
{
	namespace Assets
	{

		//TODO: Support more Shader Types.
		struct MaterialCreationDesc
		{
			IPipelineState* mPipeline = nullptr;
			ISampler* mSampler = nullptr;
		};

		struct ShaderTexture
		{
			Uint32 mSlot;
			Assets::Texture mTex;
		};

		typedef	std::vector<ShaderTexture> TextureSet;

		/*
			Material class defines how should the rendering system render the mesh,
			it contains all the info about textures & vertex, pixel shaders.
			Notes:
			   - Textures and material parsing is "currently" available in PixelShader only!
			   - Material constant buffer should contain floats only!
			   - Material Shaders should have certain order
			   - Material has two main definitions:
				 - Material Filling: means filling the material with textures.
				 - Material Creation: means processing shaders & resource heaps & graphical pipeline needed to render a mesh.

				 ---[[[ IMPORTANT NOTE: Material must be filled first before should be created it! ]]]---

		*/
		class NEAPI Material
		{
		public:
			//TODO::MATERIAL
			Material();
			~Material();

			/*
				It processes the shaders and the pipelne needed to render a mesh,
				if the pipeline equals a nullptr the default renderer pipeline is used
				and create the resource heap for all textures.
			*/
			void Create(const MaterialCreationDesc& desc);

			void BindTexSet(Uint32 index);
			void Bind();

			//PixelShader Textures
			std::vector<TextureSet> mVertexShaderTextures;
			std::vector<ShaderTexture> mVertexShaderTS;

			std::vector<TextureSet> mPixelShaderTextures;
			std::vector<ShaderTexture> mPixelShaderTS;
		private:
			IPipelineState* mPipeline = nullptr;
			RefCntAutoPtr<IShaderResourceBinding> mSRB;

			void ParseShader(SHADER_TYPE shadertype);

			/*LLGL::Buffer* mCbuffer;
			Graphics::ShaderResource mCbufferRef;
			std::map<std::string, Float32> mCbufferData;
			bool mPSHaveMaterialCB;*/
		};
	}
}