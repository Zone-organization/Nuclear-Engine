#include <Engine\Assets\Material.h>
#include <Base\Utilities\Hash.h>
#include <Engine/Assets/DefaultTextures.h>

namespace NuclearEngine
{
	namespace Assets
	{
		Material::Material()
		{
		}
		Material::~Material()
		{
		}
		void Material::SetVertexShader(Graphics::API::VertexShader vshader)
		{
			mVShader = vshader;
			ParseVertexShader();
		}
		void Material::SetPixelShader(Graphics::API::PixelShader pshader)
		{
			mPShader = pshader;
			ParsePixelShader();
		}
		void Material::SetTexture(std::string name, const Assets::Texture& value, unsigned int slot)
		{
			auto it = mPixelShaderTextures.find(Utilities::Hash(name));
			if (it != mPixelShaderTextures.end())
			{
			//	it->second.mGPUTexture = value;
			//	it->second.mSlot = slot;
			}
			else 
			{
				Log.Error("[Material] Setting an non-existing texture (" + name + ").\n");
			}
		}
		void Material::SetTexture(std::string name, const MaterialTexture & texture)
		{
			auto it = mPixelShaderTextures.find(Utilities::Hash(name));
			if (it != mPixelShaderTextures.end())
			{
			//	it->second = texture;
			}
			else
			{
				Log.Error("[Material] Setting an non-existing texture (" + name + ").\n");
			}
		}
		void Material::ParseVertexShader()
		{
			if (mVShader.isValid)
			{

			}
		}
		void Material::ParsePixelShader()
		{
			if (mPShader.isValid)
			{
				mPixelShaderTextures.clear();
				PixelShaderTS.clear();
				for (auto Tex : mPShader.Reflection.Textures)
				{
					if (Tex.first.find("NE_Tex_") == 0)
					{
						std::string texname(Tex.first);
						texname.erase(0, 7);
						MaterialTexture mTex;
						mTex.mGPUTexture = DefaultTextures::DefaultBlackTex;
						mTex.mGPUTexture.SetName(texname);
						mTex.mSlot = Tex.second.BindingSlot;
						PixelShaderTS.push_back(mTex);
					}					
				}
			}
		}
	}
}