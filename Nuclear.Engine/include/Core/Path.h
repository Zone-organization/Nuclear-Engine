#pragma once
#include <NE_Common.h>
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

			const bool isValid() const;

			const std::string& GetInputPath() const;
			const std::string& GetRealPath() const;
			std::string GetFilename(bool removeextension = false) const;
			std::string GetPathNoExt() const;

			static std::unordered_map<std::string, std::string> mReservedPaths;

		private:
			void Parse();
			std::string mRealPath;
			std::string mInputPath;
		};

	}
}