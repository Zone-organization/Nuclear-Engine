#pragma once
#include <Platform\Timer.h>

namespace NuclearEngine {
	namespace Platform {
		class NEAPI Clock
		{
		public:
			Clock();

			Timer GetElapsedTime() const;

			Timer Restart();

		private:
			Timer m_startTime; ///< Time of last reset, in microseconds
		};
	}
}