#include "NuclearCommon/Utilities/Logger.h"
#include <codecvt>
#include <locale>
#include <iostream>

namespace NuclearCommon {
	namespace Utilities {
		Logger::Logger()
		{
		}

		Logger::~Logger()
		{
		}

		void Logger::EndLine()
		{
			Write(std::string("\n"));
		}

		void Logger::Error(const char* format, ...)
		{
			Write(std::string("[Error] "));

			va_list args;
			va_start(args, format);
			Write(std::string(format));
			va_end(args);

		}

		void Logger::FatalError(const char * format, ...)
		{
			Write(std::string("[Fatal Error] "));

			va_list args;
			va_start(args, format);
			Write(std::string(format));
			va_end(args);

			//Sleep(50000);
			exit(1);
		}


		void Logger::Warning(const char* format, ...)
		{
			Write(std::string("[Warning] "));
			va_list args;
			va_start(args, format);
			Write(std::string(format));

			va_end(args);


		}

		void Logger::Error(std::string format, ...)
		{
			Write(std::string("[Error] "));
			va_list args;
			va_start(args, format);
			Write(format);

			va_end(args);
		}

		void Logger::FatalError(std::string format, ...)
		{
			Write(std::string("[Fatal Error] "));

			va_list args;
			va_start(args, format);
			//MessageBox(NULL,String2WSTR(format).c_str(), L"[FATAL_ERROR]", MB_OK | MB_ICONERROR);
			Write(format);

			
			va_end(args);

			//Sleep(50000);
			exit(1);
		}

		void Logger::Warning(std::string format, ...)
		{
			Write(std::string("[Warning] "));
			va_list args;
			va_start(args, format);
			Write(format);

			va_end(args);

		}

		void Logger::Info(std::string format, ...)
		{

			va_list args;
			va_start(args, format);
			Write(format);
			va_end(args);
		}

		void Logger::Info(const char* format, ...)
		{


			va_list args;
			va_start(args, format);
			Write(std::string(format));


			va_end(args);

		}
		void Logger::Write(std::string TextString)
		{
			printf(TextString.c_str());
		}
	}
}