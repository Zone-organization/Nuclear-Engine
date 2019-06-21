#include <Engine/Assets/Texture.h>
#include <Base\Utilities\Hash.h>

namespace NuclearEngine
{
	namespace Assets 
	{
		Texture::Texture()
			: mUsageType(Diffuse)
		{
		}

		Texture::~Texture()
		{
		}
	
		Uint8 Texture::GetUsageType()
		{
			return mUsageType;
		}
		
		void Texture::SetUsageType(Uint8 type)
		{
			mUsageType = type;
		}
		void Texture::SetUsageType(TextureUsageType type)
		{
			mUsageType = type;
		}
	}
}