#include "Core\Path.h"
#include <algorithm>
#include "Platform\FileSystem.h"

namespace Nuclear
{
	namespace Core
	{
		std::unordered_map<std::string, std::string> Path::mReservedPaths = std::unordered_map<std::string, std::string>();

		Path::Path()
		{
		}

		Path::Path(const char * path, bool ParseForReservedPaths)
		{
			mInputPath = std::string(path);
			mRealPath = mInputPath;

			if (ParseForReservedPaths)
			{
				Parse();
			}
		}

		Path::Path(const std::string & path, bool ParseForReservedPaths)
		{
			mInputPath = path;
			mRealPath = mInputPath;

			if (ParseForReservedPaths)
			{
				Parse();
			}
		}

		void Path::SetPath(const std::string& path, bool ParseForReservedPaths)
		{
			mInputPath = path;
			mRealPath = mInputPath;

			if (ParseForReservedPaths)
			{
				Parse();
			}
		}

		const bool Path::isValid() const
		{
			if (mInputPath != "")
				return true;

			//Sometimes a path can include only a real path
			if (mRealPath != "")
				return true;

			return false;
		}

		const std::string& Path::GetInputPath() const
		{
			return mInputPath;
		}

		const std::string& Path::GetRealPath() const
		{
			return mRealPath;
		}

		const bool Path::CreatePathIfDoesntExist() const
		{
			if (isValid())
			{
				std::size_t found = mRealPath.find_last_of("/\\");
				std::string directory_path = mRealPath.substr(0, found);

				return Platform::FileSystem::GetInstance().CreateFolders(directory_path);
			}
			return false;
		}

		std::string Path::GetPathNoExt() const
		{
			std::string result = mRealPath.substr(0, mRealPath.find_last_of('.'));

			return result;
		}

		std::string Path::GetParentDirectory(bool addfilename) const
		{
			std::size_t found = mRealPath.find_last_of("/\\");
			std::string directory_path = mRealPath.substr(0, found);


			std::size_t found2 = directory_path.find_last_of("/\\");
			std::string directory_path_name = directory_path.substr(found2 + 1);

			if (addfilename)
			{
				std::string filename = mRealPath.substr(found + 1);
				return directory_path_name + "/" + filename;
			}

			return directory_path_name;
		}

		std::string Path::GetFilename(bool removeextension ) const
		{
			std::string result;

			result = mRealPath.substr(mRealPath.find_last_of("/") + 1);

			if (removeextension)
			{
				result = result.substr(0, result.find_last_of('.'));
			}
			return result;
		}

		void Path::Parse()
		{
			//check if folder add '/' at the end of path
			
			if (mInputPath != "")
			{
				auto found = mInputPath.find_last_of('.');
				if (found == std::string::npos)
				{
					if (mInputPath.back() != '/')
						mInputPath.push_back('/');
				}

				for (auto& str : mReservedPaths)
				{
					auto Offset = mInputPath.find(str.first);
					if (Offset != std::string::npos)
					{
						mRealPath.replace(Offset, str.first.length(), str.second);
					}
				}
				std::replace(mInputPath.begin(), mInputPath.end(), '\\', '/');
				std::replace(mRealPath.begin(), mRealPath.end(), '\\', '/');
			}
		}
	}
}