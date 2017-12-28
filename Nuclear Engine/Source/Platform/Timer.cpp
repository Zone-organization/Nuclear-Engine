#include <Platform\Timer.h>
namespace NuclearEngine {
	namespace Platform {

		const Timer Timer::Zero;

		Timer::Timer() :
			m_microseconds(0)
		{
		}

		float Timer::AsSeconds() const
		{
			return m_microseconds / 1000000.f;
		}

		Int32 Timer::AsMilliseconds() const
		{
			return static_cast<Int32>(m_microseconds / 1000);
		}


		Int64 Timer::AsMicroseconds() const
		{
			return m_microseconds;
		}

		Timer::Timer(Int64 microseconds) :
			m_microseconds(microseconds)
		{
		}


		Timer seconds(float amount)
		{
			return Timer(static_cast<Int64>(amount * 1000000));
		}


		Timer milliseconds(Int32 amount)
		{
			return Timer(static_cast<Int64>(amount) * 1000);
		}

		Timer microseconds(Int64 amount)
		{
			return Timer(amount);
		}

		bool operator ==(Timer left, Timer right)
		{
			return left.AsMicroseconds() == right.AsMicroseconds();
		}

		bool operator !=(Timer left, Timer right)
		{
			return left.AsMicroseconds() != right.AsMicroseconds();
		}

		bool operator <(Timer left, Timer right)
		{
			return left.AsMicroseconds() < right.AsMicroseconds();
		}

		bool operator >(Timer left, Timer right)
		{
			return left.AsMicroseconds() > right.AsMicroseconds();
		}

		bool operator <=(Timer left, Timer right)
		{
			return left.AsMicroseconds() <= right.AsMicroseconds();
		}

		bool operator >=(Timer left, Timer right)
		{
			return left.AsMicroseconds() >= right.AsMicroseconds();
		}

		Timer operator -(Timer right)
		{
			return microseconds(-right.AsMicroseconds());
		}

		Timer operator +(Timer left, Timer right)
		{
			return microseconds(left.AsMicroseconds() + right.AsMicroseconds());
		}

		Timer& operator +=(Timer& left, Timer right)
		{
			return left = left + right;
		}

		Timer operator -(Timer left, Timer right)
		{
			return microseconds(left.AsMicroseconds() - right.AsMicroseconds());
		}

		Timer& operator -=(Timer& left, Timer right)
		{
			return left = left - right;
		}

		Timer operator *(Timer left, float right)
		{
			return seconds(left.AsSeconds() * right);
		}

		Timer operator *(Timer left, Int64 right)
		{
			return microseconds(left.AsMicroseconds() * right);
		}

		Timer operator *(float left, Timer right)
		{
			return right * left;
		}

		Timer operator *(Int64 left, Timer right)
		{
			return right * left;
		}

		Timer& operator *=(Timer& left, float right)
		{
			return left = left * right;
		}

		Timer& operator *=(Timer& left, Int64 right)
		{
			return left = left * right;
		}

		Timer operator /(Timer left, float right)
		{
			return seconds(left.AsSeconds() / right);
		}

		Timer operator /(Timer left, Int64 right)
		{
			return microseconds(left.AsMicroseconds() / right);
		}

		Timer& operator /=(Timer& left, float right)
		{
			return left = left / right;
		}

		Timer& operator /=(Timer& left, Int64 right)
		{
			return left = left / right;
		}

		float operator /(Timer left, Timer right)
		{
			return left.AsSeconds() / right.AsSeconds();
		}

		Timer operator %(Timer left, Timer right)
		{
			return microseconds(left.AsMicroseconds() % right.AsMicroseconds());
		}

		Timer& operator %=(Timer& left, Timer right)
		{
			return left = left % right;
		}
	}
}