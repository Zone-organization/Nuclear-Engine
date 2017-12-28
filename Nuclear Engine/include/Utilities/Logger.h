#pragma once
#include <NE_PrivateCommon.h>
#include <string>
#include <stdarg.h>

#pragma warning( disable : 4251)

namespace NuclearEngine {
	namespace Utilities {

		class NEAPI Logger
		{
		public:
			Logger();

			~Logger();

			void EndLine();

			void Error(std::string format, ...);

			void FatalError(std::string format, ...);

			void Warning(std::string format, ...);

			void Info(std::string format, ...);

			void Error(const char* format, ...);

			void FatalError(const char* format, ...);

			void Warning(const char* format, ...);

			void Info(const char* format, ...);

		private:
			void Write(std::string TextString);
		};

	}
	static Utilities::Logger *Log = new Utilities::Logger();

}
