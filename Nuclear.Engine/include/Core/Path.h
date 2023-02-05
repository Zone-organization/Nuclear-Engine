#pragma once
#include <NE_Common.h>
#include <string>
#include <unordered_map>
#include <Serialization/Access.h>
#include <Serialization/IsLoading.h>

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


			//only creates folders
			const bool CreatePathIfDoesntExist() const;

			std::string GetParentDirectory(bool addfilename) const;
			std::string GetFilename(bool removeextension = false) const;
			std::string GetPathNoExt() const;
			const std::string GetExtension() const;

			static std::unordered_map<std::string, std::string> mReservedPaths;

			constexpr static auto serialize(auto& archive, auto& self)
			{
				ISLOADING(archive)
				{
					auto result = archive(self.mInputPath);
					self.mRealPath = self.mInputPath;
					self.Parse();
					return result;
				}
				else
				{
					return archive(self.mInputPath);
				}
			}
		private:
			friend Serialization::Access;
			void Parse();
			std::string mRealPath;
			std::string mInputPath;
		};

	}
}