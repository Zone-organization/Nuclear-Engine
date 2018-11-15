#include <Engine\Assets\Material.h>
#include <Base\Utilities\Hash.h>
#include <Engine/Assets/DefaultTextures.h>

namespace NuclearEngine
{
	namespace Assets
	{
		Material::Material()
			: mPixelShaderTextures(std::vector<TextureSet>()), PixelShaderTS(std::vector<ShaderTexture>())
		{
		}
		Material::~Material()
		{
		}
		void Material::SetVertexShader(Graphics::API::VertexShader vshader)
		{
			mVShader = vshader;
		}
		void Material::SetPixelShader(Graphics::API::PixelShader pshader)
		{
			mPShader = pshader;
			ParsePixelShader();
		}

		void Material::BindTexSet(Uint32 index)
		{
			//TODO: Check if all Slots have been occupied and then bind the free ones to fix some glitches
			if (!mPixelShaderTextures.empty())
			{
				for (auto tex : mPixelShaderTextures.at(index))
				{
					tex.mTexture.PSBind(tex.mSlot);
				}
			}
			/*auto tex = mPixelShaderTextures.at(index).at(0);
			tex.mTexture.PSBind(tex.GetUsageType());*/
		}
	
		TextureUsageType ParseTexUsageFromName(std::string& name)
		{
			if (name.find("Diffuse") == 0)
				return TextureUsageType::Diffuse;
			else if (name.find("Specular") == 0)
				return TextureUsageType::Specular;
			else if (name.find("Normal") == 0)
				return TextureUsageType::Normal;

			return TextureUsageType::Unknown;
		}

		void Material::ParsePixelShader()
		{
			if (mPShader.isValid)
			{			
				//Parse Shader
				PixelShaderTS.clear();
				for (auto Tex : mPShader.Reflection.Textures)
				{
					if (Tex.first.find("NE_Tex_") == 0)
					{
						std::string texname(Tex.first);
						texname.erase(0, 7);
						ShaderTexture mTex;
						mTex.mTexture = DefaultTextures::DefaultBlackTex;
						mTex.mTexture.SetName(texname);
						mTex.mSlot = Tex.second.BindingSlot;
						mTex.mTexture.SetUsageType(ParseTexUsageFromName(texname));
						PixelShaderTS.push_back(mTex);
					}					
				}

				//Parse loaded textures
				for (int i = 0; i < mPixelShaderTextures.size(); i++)
				{
					for (int j = 0; j < mPixelShaderTextures.at(i).size(); j++)
					{
						for (ShaderTexture TSinfo : PixelShaderTS)
						{
							if (mPixelShaderTextures.at(i).at(j).mTexture.GetUsageType() == TSinfo.mTexture.GetUsageType())
							{
								mPixelShaderTextures.at(i).at(j).mSlot = TSinfo.mSlot;
							}
						}
					}
				}
			}
		}
	}
}