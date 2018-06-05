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
		struct MaterialTexture
		{
			MaterialTexture() { mSlot = 0; }
			MaterialTexture(const Assets::Texture& _tex, Uint8 _slot)
				: mGPUTexture(_tex) , mSlot(_slot)
			{
			}
			Assets::Texture mGPUTexture;
			Uint8 mSlot;
		};

		typedef	std::vector<MaterialTexture> TextureSet;
			
		/*
			Material class defines how should the rendering system render the mesh,
			it contains all the info about textures & vertex, pixel shaders.
			Note: Textures and material parsing is available in PixelShader only "currently"!
		*/			
		class NEAPI Material
		{
		public:
			Material();
			~Material();

			void SetVertexShader(Graphics::API::VertexShader vshader);
			void SetPixelShader(Graphics::API::PixelShader pshader);
			void SetTexture(std::string name, const Assets::Texture& value, unsigned int slot = 0);
			void SetTexture(std::string name, const MaterialTexture& texture);

			//If the shader isn't valid the default shader is bound
			Graphics::API::VertexShader mVShader;
			Graphics::API::PixelShader mPShader;

			//PixelShader Textures
			std::vector<TextureSet> mPixelShaderTextures;
			TextureSet PixelShaderTS;
		private:
			void ParseVertexShader();
			void ParsePixelShader();
			Graphics::API::ConstantBuffer mCbuffer;

		};
	}
}