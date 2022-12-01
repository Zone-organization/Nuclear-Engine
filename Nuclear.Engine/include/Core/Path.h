#pragma once
#include <Core\NE_Common.h>
#include <string>
#include <unordered_map>

namespace Nuclear
{
	namespace Core
	{
		class NEAPI Path
		{
		public:
			Path();
			Path(const char* path, bool ParseForReservedPaths = true);
			Path(const std::string& path, bool ParseForReservedPaths = true);
			
			void SetPath(const std::string& path, bool ParseForReservedPaths = true);

			const std::string& GetInputPath() const;
			const std::string& GetRealPath() const;

			static std::unordered_map<std::string, std::string> mReservedPaths;

		private:
			void Parse();
			std::string mRealPath;
			std::string mInputPath;
		};

	}
}