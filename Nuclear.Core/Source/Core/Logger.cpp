#include <Core\Logger.h>
#include <codecvt>
#include <locale>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <Core\NE_Common.h>
namespace Nuclear
{
	namespace Core 
	{
		std::wstring String2WSTR(const std::string& s)
		{
#ifdef _WIN32
			int len;
			int slength = (int)s.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
#endif
		}
		void SetConsoleColor(int colorCode)
		{
#ifdef _WIN32

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

		void Logger::Error(const std::string& format)
		{
			SetConsoleColor(12);
			Write(std::string("[Error] "));
			Write(format);
			SetConsoleColor(15);
		}

		void Logger::FatalError(const std::string& format)
		{
			SetConsoleColor(79);
			std::string info("Nuclear Engine has encountred a fatal error, and have to exit, please inform the developers for more support.\nDetails: ");

			Write(std::string("[FATAL_ERROR] "));
			Write(format);
#ifdef _WIN32
			MessageBox(NULL, String2WSTR(info).c_str(), L"[FATAL_ERROR]", MB_OK | MB_ICONERROR);
#endif
			exit(1);
		}

		void Logger::Warning(const std::string& format)
		{
			SetConsoleColor(14);
			Write(std::string("[Warning] "));
			Write(format);
			SetConsoleColor(15);

		}

		void Logger::Info(const std::string& format)
		{
			SetConsoleColor(15);
			Write(format);
		}

		void Logger::Write(const std::string& TextString)
		{
			printf(TextString.c_str());
		}
	}
}