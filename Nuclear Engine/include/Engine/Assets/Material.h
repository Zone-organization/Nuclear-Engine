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
			Notes:
			   - Textures and material parsing is "currently" available in PixelShader only!
			   - Material constant buffer should contain floats only!
		*/			
		class NEAPI Material
		{
		public:
			Material();
			~Material();

			void SetVertexShader(Graphics::API::VertexShader vshader);
			void SetPixelShader(Graphics::API::PixelShader pshader);

			void BindTexSet(Uint32 index);
			void Bind();

			//If the shader isn't valid the default shader is bound
			Graphics::API::VertexShader mVShader;
			Graphics::API::PixelShader mPShader;

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
			void ParsePixelShader();
			Graphics::API::ConstantBuffer mCbuffer;
			Graphics::API::Reflected_Constantbuffer mCbufferRef;
			std::map<std::string, Float32> mCbufferData;
			bool mPSHaveMaterialCB;
		};
	}
}