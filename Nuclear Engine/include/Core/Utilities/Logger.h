#pragma once
#include <Core\NE_PrivateCommon.h>
#include <string>
#include <stdarg.h>

#pragma warning( disable : 4251)

namespace NuclearEngine {
	namespace Core {
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

			private:
				void Write(std::string TextString);
			};

		}
	}
		static Core::Utilities::Logger Log;
}