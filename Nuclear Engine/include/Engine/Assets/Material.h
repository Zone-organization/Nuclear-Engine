#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets\Texture.h>
#include <Engine\Graphics\ShaderTypes.h>
#include <Engine/Graphics\Shader.h>
#include <LLGL/Buffer.h>
#include <map>

namespace NuclearEngine
{
	namespace Assets
	{
	
		struct ShaderTexture
		{
			ShaderTexture() { mSlot = 0; }
			ShaderTexture(Assets::Texture* _tex, Uint8 _slot = 14)	: mTexture(_tex) , mSlot(_slot)	{	}
			LLGL::ResourceHeap* mResourceHeap;
			Assets::Texture* mTexture;
			Uint8 mSlot;
		};
	
		typedef	std::vector<Assets::ShaderTexture> TextureSet;

		//TODO: Support more Shader Types.
		struct MaterialCreationDesc
		{
			Graphics::Shader* mVShader = nullptr;
			Graphics::Shader* mPShader = nullptr;
			LLGL::GraphicsPipeline* mPipeline = nullptr;
			LLGL::Sampler* mSampler = nullptr;
		};

		/*
			Material class defines how should the rendering system render the mesh,
			it contains all the info about textures & vertex, pixel shaders.
			Notes:
			   - Textures and material parsing is "currently" available in PixelShader only!
			   - Material constant buffer should contain floats only!
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
			LLGL::GraphicsPipeline*	mPipeline = nullptr;

			void SetMaterialVariable(const std::string&  name, Float32 value);
			void SetMaterialVariable(const std::string&  name, Math::Vector2 value);
			void SetMaterialVariable(const std::string&  name, Math::Vector3 value);
			void SetMaterialVariable(const std::string&  name, Math::Vector4 value);
			void SetMaterialVariable(const std::string&  name, Math::Matrix2 value);
			void SetMaterialVariable(const std::string&  name, Math::Matrix3 value);
			void SetMaterialVariable(const std::string&  name, Math::Matrix4 value);
			void UpdateMaterialCBuffer();

			//PixelShader Textures
			std::vector<TextureSet> mPixelShaderTextures;
			std::vector<ShaderTexture> PixelShaderTS;
		private:
			void ParsePixelShader(Graphics::Shader* PixelShader);
			LLGL::Buffer* mCbuffer;
			Graphics::Reflected_Constantbuffer mCbufferRef;
			std::map<std::string, Float32> mCbufferData;
			bool mPSHaveMaterialCB;
		};
	}
}