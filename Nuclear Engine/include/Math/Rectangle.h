#pragma once
#include <NE_Common.h>

namespace NuclearEngine {
	namespace Math {

		struct NEAPI Rectangle
		{
			int x, y, width, height;

			Rectangle() :
				x(0), y(0), width(0), height(0)
			{
			}
			Rectangle(int x, int y, int width, int height) :
				x(x), y(y), width(width), height(height)
			{
			}
			bool Intersect(const Rectangle& rectangle) {
				if (y < rectangle.y + rectangle.height && y + height > rectangle.y && x + width > rectangle.x && x < rectangle.x + rectangle.width)
					return true;

				return false;
			}

			friend Rectangle operator+(const Rectangle& leftRectangle, const Rectangle& rightRectangle)
			{
				return Rectangle(leftRectangle.x + rightRectangle.x, leftRectangle.y + rightRectangle.y, leftRectangle.width + rightRectangle.width, leftRectangle.height + rightRectangle.height);
			}

			friend Rectangle operator-(const Rectangle& leftRectangle, const Rectangle& rightRectangle)
			{
				return Rectangle(leftRectangle.x - rightRectangle.x, leftRectangle.y - rightRectangle.y, leftRectangle.width - rightRectangle.width, leftRectangle.height * rightRectangle.height);
			}

			friend Rectangle operator*(const Rectangle& leftRectangle, const Rectangle& rightRectangle)
			{
				return Rectangle(leftRectangle.x * rightRectangle.x, leftRectangle.y * rightRectangle.y, leftRectangle.width * rightRectangle.width, leftRectangle.height * rightRectangle.height);
			}

			friend Rectangle operator/(const Rectangle& leftRectangle, const Rectangle& rightRectangle)
			{
				return Rectangle(leftRectangle.x / rightRectangle.x, leftRectangle.y / rightRectangle.y, leftRectangle.width / rightRectangle.width, leftRectangle.height / rightRectangle.height);
			}

			/*friend ostream& operator<<(ostream& os, const Rectangle& rectangle)
			{
				return os << "{" << rectangle.x << ", " << rectangle.y << ", " << rectangle.width << ", " << rectangle.height << "}";
			}*/
		};
	}
}