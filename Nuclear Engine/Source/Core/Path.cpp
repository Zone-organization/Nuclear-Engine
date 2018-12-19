#include "Core\Path.h"

namespace NuclearEngine
{
	namespace Core
	{
		std::map<std::string, std::string> Path::mReservedPaths = std::map<std::string, std::string>();

		Path::Path()
		{
		}

		Path::Path(const char * path, bool ParseForReservedPaths)
		{
			mPath = std::string(path);

			if (ParseForReservedPaths)
			{
				Parse();
			}
		}

		Path::Path(const std::string & path, bool ParseForReservedPaths)
		{
			mPath = path;

			if (ParseForReservedPaths)
			{
				Parse();
			}
		}
		void Path::Parse()
		{
			for (auto str : mReservedPaths)
			{
				auto Offset = mPath.find(str.first);
				if (Offset != std::string::npos)
				{
					mPath.replace(Offset, str.second.length(), str.second);
				}
			}
		}
	}
}