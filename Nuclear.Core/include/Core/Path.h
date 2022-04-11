#pragma once
#include <Core\NE_Common.h>
#include <string>
#include <unordered_map>

namespace Nuclear
{
	namespace Core
	{
		class Path
		{
		public:
			Path();
			Path(const char* path, bool ParseForReservedPaths = true);
			Path(const std::string& path, bool ParseForReservedPaths = true);
			
			std::string mRealPath;
			std::string mInputPath;

			static std::unordered_map<std::string, std::string> mReservedPaths;

		private:
			void Parse();
		};

	}
}