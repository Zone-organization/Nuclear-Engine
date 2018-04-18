#include <Assets\Material.h>

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
		void Material::SetTexture(std::string name, Graphics::API::Texture * value, unsigned int slot)
		{
			auto it = mTextures.find(name);
			if (it != mTextures.end())
			{
				it->second.mGPUTexture = value;
				it->second.mSlot = slot;

				//mymap.erase(it);
			}
			else {

			}
		}
		void Material::SetTexture(std::string name, const MaterialTexture & texture)
		{
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
				mTextures.clear();
				for (auto Tex : mPShader.Reflection.Textures)
				{
					if (Tex.first.find("Material_") == 0)
					{
						mTextures[Tex.first] = MaterialTexture(nullptr, Tex.second.BindingSlot);
					}					
				}
			}
		}
	}
}