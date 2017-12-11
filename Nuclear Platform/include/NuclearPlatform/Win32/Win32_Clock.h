#pragma once
#include <NuclearPlatform\Timer.h>

namespace NuclearPlatform {
	namespace Internals {
		class Win32_Clock
		{
		public:

			static Timer getCurrentTime();
		};
	}
}