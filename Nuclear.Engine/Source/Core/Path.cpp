#include "Core\Path.h"
#include <algorithm>

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

		const std::string& Path::GetInputPath() const
		{
			return mInputPath;
		}

		const std::string& Path::GetRealPath() const
		{
			return mRealPath;
		}

		std::string Path::GetPathNoExt() const
		{
			std::string result = mRealPath.substr(0, mRealPath.find_last_of('.'));

			return result;
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