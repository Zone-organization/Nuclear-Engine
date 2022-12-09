#pragma once
#include <NE_Common.h>
#include <chrono>

namespace Nuclear
{
	namespace Utilities 
	{
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