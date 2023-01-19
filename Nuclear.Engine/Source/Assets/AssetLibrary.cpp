#include <Assets/AssetLibrary.h>
#include <filesystem>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Assets
	{
		AssetLibrary::AssetLibrary()
		{
		}
	
		bool CreateDir(const std::string& dirName)
		{
			if (std::filesystem::exists(dirName))
			{
				return true;
			}
			std::error_code err;
			if (!std::filesystem::create_directories(dirName, err))
			{
				NUCLEAR_WARN("[AssetLibrary] Failed to create directory: {0} Error: {1}", dirName ,err.message());
				return false;
			}

			return true;
		}

		void AssetLibrary::Initialize(const std::string& path)
		{
			if (path.back() != '/')
			{
				mPath = path + "/";
			}
			else
			{
				mPath = path;
			}

			std::string assets[9] = { "Textures",				//Importing & Loading done
				"Models",
				"Meshs",
				"Materials",
				"Animations",
				"AudioClips",
				"Fonts",
				"Shaders",
				"Scripts" };

			for (int i = 0; i < 9; i++)
			{
				CreateDir(mPath + assets[i]);
			}
		}

		const std::string& AssetLibrary::GetPath() const
		{
			return mPath;
		}

		AssetLibrary& AssetLibrary::GetInstance()
		{
			static AssetLibrary instance;

			return instance;
		}
	}
}