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
						ShaderTextureInfo mTex;
						mTex.mDefaultTexture = DefaultTextures::DefaultBlackTex;
						mTex.mDefaultTexture.SetName(texname);
						mTex.mSlot = Tex.second.BindingSlot;
						PixelShaderTS.push_back(mTex);
					}					
				}
			}
		}
	}
}