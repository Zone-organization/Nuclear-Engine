#pragma once
#include <Core\NE_Common.h>
#include <string>
#include <stdarg.h>

#pragma warning( disable : 4251)

namespace Nuclear {
	namespace Core {

		class Logger
		{
		public:
			Logger();

			~Logger();

			void EndLine();

			void Error(std::string format, ...);

			void FatalError(std::string format, ...);

			void Warning(std::string format, ...);

			void Info(std::string format, ...);

		private:
			void Write(std::string TextString);
		};
	}
	static Core::Logger Log;
}