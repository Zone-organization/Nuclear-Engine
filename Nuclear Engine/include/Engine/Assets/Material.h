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

		struct ShaderTexture
		{
			ShaderTexture() { mSlot = 0; }
			ShaderTexture(Assets::Texture* _tex, Uint8 _slot = 14) : mTexture(_tex), mSlot(_slot) {	}
			//LLGL::ResourceHeap* mResourceHeap;
			Assets::Texture* mTexture;
			Uint8 mSlot;
		};

		typedef	std::vector<Assets::ShaderTexture> TextureSet;

		//TODO: Support more Shader Types.
		struct MaterialCreationDesc
		{
			IShader* mVShader = nullptr;
			IShader* mPShader = nullptr;
			IPipelineState* mPipeline = nullptr;
			ISampler* mSampler = nullptr;
		};

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

			//If the shader isn't valid the default shader is bound
			//LLGL::GraphicsPipeline*	mPipeline = nullptr;

			void UpdateMaterialCBuffer();

			/*
				Creates PipelineLayoutDescriptor but u should consider some restrictions when writing shaders for it:
				Order of Resources:
				 - Textures
				 - Samplers
				 - Constant Buffer

			EX:
					Texture2D Tex_X;
					Texture2D Tex_Y;
					
					SamplerState Sampler_A;
					SamplerState Sampler_B;

					cbuffer myCB_1;
					cbuffer myCB_2;
			*/
			//static LLGL::PipelineLayoutDescriptor GeneratePipelineLayoutDesc(Graphics::Shader* _VShader, Graphics::Shader* _PShader);

			//PixelShader Textures
			std::vector<TextureSet> mPixelShaderTextures;
			std::vector<ShaderTexture> PixelShaderTS;
		private:
			void ParsePixelShader(IShader* PixelShader);
			/*LLGL::Buffer* mCbuffer;
			Graphics::ShaderResource mCbufferRef;
			std::map<std::string, Float32> mCbufferData;
			bool mPSHaveMaterialCB;*/
		};
	}
}