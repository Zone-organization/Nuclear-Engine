#include "Serialization/SerializationEngine.h"



namespace Nuclear
{
	namespace Serialization
	{
		inline SerializationEngine& SerializationEngine::GetInstance() {
			static SerializationEngine instance;

			return instance;
		}
		bool SerializationEngine::SaveScene()
		{
			return false;
		}
		bool SerializationEngine::SaveAssetLibraryInfo(Assets::AssetLibrary* lib)
		{
			return false;
		}
		SerializationEngine::SerializationEngine()
		{
		}
	}
}