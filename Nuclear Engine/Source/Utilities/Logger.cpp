#include <Utilities/Logger.h>
#include <codecvt>
#include <locale>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#endif
namespace NuclearEngine {
	namespace Utilities {

		void SetConsoleColor(int colorCode)
		{
#ifdef WIN32

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, colorCode);
#endif
		}

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

		void Logger::Error(std::string format, ...)
		{
			SetConsoleColor(12);
			Write(std::string("[Error] "));
			va_list args;
			va_start(args, format);
			Write(format);

			va_end(args);
		}

		void Logger::FatalError(std::string format, ...)
		{
			SetConsoleColor(79);
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
			SetConsoleColor(14);
			Write(std::string("[Warning] "));
			va_list args;
			va_start(args, format);
			Write(format);

			va_end(args);

		}

		void Logger::Info(std::string format, ...)
		{
			SetConsoleColor(15);

			va_list args;
			va_start(args, format);
			Write(format);
			va_end(args);
		}

		void Logger::Write(std::string TextString)
		{
			printf(TextString.c_str());
		}
	}
}