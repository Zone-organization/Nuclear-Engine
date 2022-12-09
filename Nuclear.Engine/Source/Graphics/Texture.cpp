#include <Graphics/Texture.h>
#include <Utilities\Hash.h>

namespace Nuclear
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