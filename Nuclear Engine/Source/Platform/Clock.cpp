#include <Platform\Clock.h>

#ifdef NUCLEAR_PLATFORM_WINDOWS_PC
#include <Platform\Win32\Win32_Clock.h>
typedef NuclearEngine::Platform::Internals::Win32_Clock ClockImpl;
#endif

namespace NuclearEngine {
	namespace Platform {

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
}