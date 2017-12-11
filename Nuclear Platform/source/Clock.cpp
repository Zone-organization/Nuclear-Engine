#include <NuclearPlatform\Clock.h>

#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include <NuclearPlatform\Win32\Win32_Clock.h>
typedef NuclearPlatform::Internals::Win32_Clock ClockImpl;
#endif

namespace NuclearPlatform {

	Clock::Clock() :
		m_startTime(ClockImpl::getCurrentTime())
	{
	}



	Timer Clock::GetElapsedTime() const
	{
		return ClockImpl::getCurrentTime() - m_startTime;
	}



	Timer Clock::Restart()
	{
		Timer now = ClockImpl::getCurrentTime();
		Timer elapsed = now - m_startTime;
		m_startTime = now;

		return elapsed;
	}
}