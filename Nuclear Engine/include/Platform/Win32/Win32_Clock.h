#pragma once
#include <Platform\Timer.h>

namespace NuclearEngine {
	namespace Platform {
		namespace Internals {
			class Win32_Clock
			{
			public:

				static Timer getCurrentTime();
			};
		}
	}
}