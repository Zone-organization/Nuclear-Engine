#pragma once
#include <Core\NE_Common.h>
#include <chrono>

namespace Nuclear {
	namespace Core {
		namespace Utilities {
			class Timer
			{
			public:

				Timer();
				void Reset();

				double GetElapsedTimeInNanoseconds();
				double GetElapsedTimeInMilliseconds();
				double GetElapsedTimeInSeconds();
			private:
				std::chrono::time_point<std::chrono::steady_clock> start;
			};
		}
	}
}