#pragma once
#include <NE_Common.h>
namespace NuclearEngine {
	namespace API {

		struct NEAPI Color
		{
			float r, g, b, a;

			Color();
			Color(float value);
			Color(int Red, int Green, int Blue, int Alpha);
			Color(float r, float g, float b, float a);
			~Color();

			friend NEAPI Color operator+(const Color& left, const Color& right);
			friend NEAPI Color operator-(const Color& left, const Color& right);
			friend NEAPI Color operator*(const Color& left, const Color& right);
			friend NEAPI Color operator/(const Color& left, const Color& right);

			Color& operator+=(const Color& right);
			Color& operator-=(const Color& right);
			Color& operator*=(const Color& right);
			Color& operator/=(const Color& right);

			bool operator==(const Color& right);
			bool operator!=(const Color& right);

			friend NEAPI std::ostream& operator<<(std::ostream& os, const Color& color);
		};
	}
}