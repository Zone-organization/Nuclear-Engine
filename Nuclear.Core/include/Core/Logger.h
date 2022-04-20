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

			void Error(const std::string& format);

			void FatalError(const std::string &format);

			void Warning(const std::string& format);

			void Info(const std::string& format);

		private:
			void Write(const std::string& TextString);
		};
	}
	static Core::Logger Log;
}