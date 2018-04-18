#pragma once
#include <NE_Common.h>
#include <Graphics/API/VertexShader.h>
#include <Graphics/API/PixelShader.h>
#include <Graphics/API/ConstantBuffer.h>
#include <Graphics/API/Texture.h>
#include <map>

namespace NuclearEngine
{
	namespace Assets
	{
		struct MaterialTexture
		{
			MaterialTexture(){}
			MaterialTexture(Graphics::API::Texture* tex, Uint8 slot)
			{
				mGPUTexture = tex;
				mSlot = slot;
			}

			Graphics::API::Texture* mGPUTexture;
			Uint8 mSlot;
		};

		
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
			void SetTexture(std::string name, Graphics::API::Texture* value, unsigned int slot = 0);
			void SetTexture(std::string name, const MaterialTexture& texture);

			//If the shader isn't valid the default shader is bound
			Graphics::API::VertexShader mVShader;
			Graphics::API::PixelShader mPShader;

			//PixelShader Textures
			std::unordered_map<std::string, MaterialTexture> mTextures;

		private:
			void ParseVertexShader();
			void ParsePixelShader();
			Graphics::API::ConstantBuffer mCbuffer;

		};
	}
}