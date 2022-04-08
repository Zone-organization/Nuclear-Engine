#include <Engine/Graphics/Texture.h>
#include <Base\Utilities\Hash.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		Texture::Texture()
			: mUsageType(Unknown)
		{
		}

		Texture::~Texture()
		{
		}
	
		Uint8 Texture::GetUsageType() const
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