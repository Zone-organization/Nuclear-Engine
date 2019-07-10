#pragma once
#include <Base\NE_PrivateCommon.h>
#include <string>
#include <unordered_map>

namespace NuclearEngine
{
	namespace Core
	{
		class NEAPI Path
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