#pragma once
#include <Assets/ImageData.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Utilities/DataBlob.h>
#include <Graphics/TextureUsageType.h>

namespace Nuclear
{
	namespace Assets
	{

		struct ImageLoadingDesc
		{
			std::string mPath = "";

			Utilities::DataBlob mData;
		};

	
	}
}