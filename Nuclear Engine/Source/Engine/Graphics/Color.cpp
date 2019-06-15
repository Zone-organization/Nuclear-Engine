#include <Engine/Graphics/Color.h>

namespace NuclearEngine {
	namespace Graphics {

		Color::Color() :
			r(0.0f), g(0.0f), b(0.0f), a(0.0f)
		{
		}

		Color::Color(float value) :
			r(value), g(value), b(value), a(value)
		{
		}

		Color::Color(int Red, int Green, int Blue, int Alpha)
		{
			r = Red / 255.0f;
			g = Green / 255.0f;
			b = Blue / 255.0f;
			a = Alpha / 255.0f;
		}

		Color::Color(float r, float g, float b, float a) :
			r(r), g(g), b(b), a(a)
		{
		}

		Color::~Color()
		{
		}

		Math::Vector4 Color::Get4f()
		{
			return Math::Vector4(r,g,b,a);
		}

		Color operator+(const Color& left, const Color& right)
		{
			return Color(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a);
		}

		Color operator-(const Color& left, const Color& right)
		{
			return Color(left.r - right.r, left.g - right.g, left.b - right.b, left.a - right.a);
		}

		Color operator*(const Color& left, const Color& right)
		{
			return Color(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a);
		}

		Color operator/(const Color& left, const Color& right)
		{
			return Color(left.r / right.r, left.g / right.g, left.b / right.b, left.a / right.a);
		}

		Color & Color::operator=(const Color & right)
		{
			this->r = right.r;
			this->g = right.g;
			this->b = right.b;
			this->a = right.a;
			return *this;
		}

		Color& Color::operator+=(const Color& right)
		{
			this->r += right.r;
			this->g += right.g;
			this->b += right.b;
			this->a += right.a;
			return *this;
		}

		Color& Color::operator-=(const Color& right)
		{
			this->r -= right.r;
			this->g -= right.g;
			this->b -= right.b;
			this->a -= right.a;
			return *this;
		}

		Color& Color::operator*=(const Color& right)
		{
			this->r *= right.r;
			this->g *= right.g;
			this->b *= right.b;
			this->a *= right.a;
			return *this;
		}

		Color& Color::operator/=(const Color& right)
		{
			this->r /= right.r;
			this->g /= right.g;
			this->b /= right.b;
			this->a /= right.a;
			return *this;
		}

		bool Color::operator==(const Color &right)
		{
			return r == right.r && g == right.g && b == right.b && a == right.a ? true : false;
		}

		bool Color::operator!=(const Color &right)
		{
			return r != right.r || g != right.g || b != right.b || a != right.a ? true : false;
		}

		std::ostream& operator<<(std::ostream& os, const Color& color)
		{
			//<< "{ " << color.r << ", " << color.g << ", " << color.b << ", " << color.a << " }"
			return os;
		}
	}
}