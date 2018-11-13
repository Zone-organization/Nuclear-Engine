#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/API/VertexShader.h>
#include <Engine/Graphics/API/PixelShader.h>
#include <Engine/Graphics/API/ConstantBuffer.h>
#include <Engine/Assets/Texture.h>
#include <map>

namespace NuclearEngine
{
	namespace Assets
	{
	
		struct ShaderTexture
		{
			ShaderTexture() { mSlot = 0; }
			ShaderTexture(const Assets::Texture& _tex, Uint8 _slot = 14)	: mTexture(_tex) , mSlot(_slot)	{	}
			Assets::Texture mTexture;
			Uint8 mSlot;
		};

		typedef	std::vector<Assets::ShaderTexture> TextureSet;
			
		/*
			Material class defines how should the rendering system render the mesh,
			it contains all the info about textures & vertex, pixel shaders.
			Note: Textures and material parsing is "currently" available in PixelShader only!
		*/			
		class NEAPI Material
		{
		public:
			Material();
			~Material();

			void SetVertexShader(Graphics::API::VertexShader vshader);
			void SetPixelShader(Graphics::API::PixelShader pshader);

			void BindTexSet(Uint32 index);

			//If the shader isn't valid the default shader is bound
			Graphics::API::VertexShader mVShader;
			Graphics::API::PixelShader mPShader;

			//PixelShader Textures
			std::vector<TextureSet> mPixelShaderTextures;
			std::vector<ShaderTexture> PixelShaderTS;
		private:
			void ParsePixelShader();
			Graphics::API::ConstantBuffer mCbuffer;

		};
	}
}