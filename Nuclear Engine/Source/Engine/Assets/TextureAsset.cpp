#include <Engine/Assets/Texture.h>
#include <Base\Utilities\Hash.h>

namespace NuclearEngine
{
	namespace Assets 
	{
		Texture::Texture()
		{
		}

		Texture::~Texture()
		{
		}
		Uint32 Texture::GetHashedName()
		{
			return mHashedName;
		}

		Uint8 Texture::GetUsageType()
		{
			return mUsageType;
		}

		void Texture::SetHashedName(Uint32 name)
		{
			mHashedName = name;
		}

		void Texture::SetName(const std::string & str)
		{
			mHashedName = Utilities::Hash(str);
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