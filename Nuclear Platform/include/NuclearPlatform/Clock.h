#pragma once
#include <NuclearPlatform\Timer.h>

namespace NuclearPlatform {

	class NPAPI Clock
	{
	public:
		Clock();

		Timer GetElapsedTime() const;

		Timer Restart();

	private:
		Timer m_startTime; ///< Time of last reset, in microseconds
	};
}