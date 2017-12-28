#pragma once
#include <NE_Common.h>

namespace NuclearEngine {
	namespace Platform {
		class NEAPI Timer
		{
		public:

			Timer();

			float AsSeconds() const;

			Int32 AsMilliseconds() const;
			Int64 AsMicroseconds() const;

			static const Timer Zero; ///< Predefined "zero" Timer value

		private:
			friend NEAPI Timer seconds(float);
			friend NEAPI Timer milliseconds(Int32);
			friend NEAPI Timer microseconds(Int64);

			explicit Timer(Int64 microseconds);

		private:

			Int64 m_microseconds; ///< Timer value stored as microseconds
		};

		NEAPI Timer seconds(float amount);

		NEAPI Timer milliseconds(Int32 amount);

		NEAPI Timer microseconds(Int64 amount);

		NEAPI bool operator ==(Timer left, Timer right);

		NEAPI bool operator !=(Timer left, Timer right);

		NEAPI bool operator <(Timer left, Timer right);

		NEAPI bool operator >(Timer left, Timer right);

		NEAPI bool operator <=(Timer left, Timer right);

		NEAPI bool operator >=(Timer left, Timer right);

		NEAPI Timer operator -(Timer right);

		NEAPI Timer operator +(Timer left, Timer right);

		NEAPI Timer& operator +=(Timer& left, Timer right);

		NEAPI Timer operator -(Timer left, Timer right);

		NEAPI Timer& operator -=(Timer& left, Timer right);

		NEAPI Timer operator *(Timer left, float right);

		NEAPI Timer operator *(Timer left, Int64 right);

		NEAPI Timer operator *(float left, Timer right);

		NEAPI Timer operator *(Int64 left, Timer right);

		NEAPI Timer& operator *=(Timer& left, float right);

		NEAPI Timer& operator *=(Timer& left, Int64 right);

		NEAPI Timer operator /(Timer left, float right);

		NEAPI Timer operator /(Timer left, Int64 right);

		NEAPI Timer& operator /=(Timer& left, float right);

		NEAPI Timer& operator /=(Timer& left, Int64 right);

		NEAPI float operator /(Timer left, Timer right);

		NEAPI Timer operator %(Timer left, Timer right);

		NEAPI Timer& operator %=(Timer& left, Timer right);
	}
}