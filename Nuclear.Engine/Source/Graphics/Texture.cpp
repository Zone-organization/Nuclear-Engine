#include <Graphics/Texture.h>
#include <Utilities\Hash.h>

namespace Nuclear
{
	namespace Graphics
	{
		Texture::Texture()
			: mTextureImage(nullptr) , mUsageType(Unknown)
		{
		}

		Texture::Texture(Assets::Image* image, TextureUsageType type)
			: mTextureImage(image), mUsageType(type)
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

		void Texture::Set(Assets::Image* image, TextureUsageType type)
		{
			SetImage(image);
			SetUsageType(type);
		}

		Assets::Image* Texture::GetImage() 
		{
			return mTextureImage; 
		}
		void Texture::SetImage(Assets::Image* image) 
		{ 
			mTextureImage = image;
		}

		Uint32 Texture::GetName() 
		{ 
			return mHashedName; 
		}

		std::string Texture::GetStringName() 
		{ 
			return mName; 
		}

		void Texture::SetName(const std::string& str)
		{
			mHashedName = Utilities::Hash(str);
			mName = str;
		}

	}
}