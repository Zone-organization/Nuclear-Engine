#include "Core\Path.h"

namespace NuclearEngine
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

		void Path::Parse()
		{
			for (auto str : mReservedPaths)
			{
				auto Offset = mInputPath.find(str.first);
				if (Offset != std::string::npos)
				{
					mRealPath.replace(Offset, str.first.length(), str.second);
				}
			}
		}
	}
}