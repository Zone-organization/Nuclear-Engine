#include <Assets/AssetLibrary.h>

namespace Nuclear
{
	namespace Assets
	{
		AssetLibrary::AssetLibrary()
		{
		}
	
		AssetLibrary& AssetLibrary::GetInstance()
		{
			static AssetLibrary instance;

			return instance;
		}
	}
}