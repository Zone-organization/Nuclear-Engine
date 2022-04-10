#pragma once
#include <Core\NE_Common.h>
#include <chrono>

namespace NuclearEngine {
	namespace Core {
		namespace Utilities {
			class NEAPI Timer
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