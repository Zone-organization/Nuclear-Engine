#include <Base\Utilities/Logger.h>
#include <codecvt>
#include <locale>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#endif
#include <Base\NE_Common.h>
namespace NuclearEngine {
	namespace Base {
		namespace Utilities {
			std::wstring String2WSTR(const std::string& s)
			{
#ifdef WIN32
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
				std::string info("Nuclear Engine has encountred a fatal error, and have to exit, please inform the developers for more support.\nDetails: ");

				Write(std::string("[FATAL_ERROR] "));

				va_list args;
				va_start(args, format);
				info = info + format;
				Write(format);
				va_end(args);
#ifdef WIN32
				MessageBox(NULL, String2WSTR(info).c_str(), L"[FATAL_ERROR]", MB_OK | MB_ICONERROR);
#endif
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
}