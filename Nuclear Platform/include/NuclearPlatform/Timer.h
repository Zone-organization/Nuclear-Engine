#pragma once
#include <NuclearPlatform\NP_Common.h>

namespace NuclearPlatform {

	class NPAPI Timer
	{
	public:

		Timer();

		float AsSeconds() const;

		Int32 AsMilliseconds() const;
		Int64 AsMicroseconds() const;

		static const Timer Zero; ///< Predefined "zero" Timer value

	private:
		friend NPAPI Timer seconds(float);
		friend NPAPI Timer milliseconds(Int32);
		friend NPAPI Timer microseconds(Int64);

		explicit Timer(Int64 microseconds);

	private:

		Int64 m_microseconds; ///< Timer value stored as microseconds
	};

	NPAPI Timer seconds(float amount);

	NPAPI Timer milliseconds(Int32 amount);

	NPAPI Timer microseconds(Int64 amount);

	NPAPI bool operator ==(Timer left, Timer right);

	NPAPI bool operator !=(Timer left, Timer right);

	NPAPI bool operator <(Timer left, Timer right);

	NPAPI bool operator >(Timer left, Timer right);

	NPAPI bool operator <=(Timer left, Timer right);

	NPAPI bool operator >=(Timer left, Timer right);

	NPAPI Timer operator -(Timer right);

	NPAPI Timer operator +(Timer left, Timer right);

	NPAPI Timer& operator +=(Timer& left, Timer right);

	NPAPI Timer operator -(Timer left, Timer right);

	NPAPI Timer& operator -=(Timer& left, Timer right);

	NPAPI Timer operator *(Timer left, float right);

	NPAPI Timer operator *(Timer left, Int64 right);

	NPAPI Timer operator *(float left, Timer right);

	NPAPI Timer operator *(Int64 left, Timer right);

	NPAPI Timer& operator *=(Timer& left, float right);

	NPAPI Timer& operator *=(Timer& left, Int64 right);

	NPAPI Timer operator /(Timer left, float right);

	NPAPI Timer operator /(Timer left, Int64 right);

	NPAPI Timer& operator /=(Timer& left, float right);

	NPAPI Timer& operator /=(Timer& left, Int64 right);

	NPAPI float operator /(Timer left, Timer right);

	NPAPI Timer operator %(Timer left, Timer right);

	NPAPI Timer& operator %=(Timer& left, Timer right);
}