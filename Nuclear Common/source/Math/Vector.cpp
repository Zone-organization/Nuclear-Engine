#include "..\..\include\NuclearCommon\Math\Vector.h"

namespace NuclearCommon {
	namespace Math {

		//VECTOR 2 IMPLEMENTATION
		Vector2::Vector2()
		{
			data = {};
		}
		Vector2::Vector2(const float & v)
		{
			data = { v, v };
		}
		Vector2::Vector2(const std::initializer_list<float> args)
		{
			assert(args.size() <= 2);
			int index = 0;
			for (auto begin = args.begin(); begin != args.end(); ++begin) {
				data.at(index++) = *begin;
			}
		}
		Vector2::Vector2(const float & x, const float & y)
		{
			data = { x, y };
		}

		Vector2 operator+(const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x + right.x, left.y + right.y);
		}

		Vector2 operator-(const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x - right.x, left.y - right.y);
		}

		Vector2 operator*(const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x * right.x, left.y * right.y);
		}

		Vector2 operator/(const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x / right.x, left.y / right.y);
		}

		Vector2& Vector2::operator+=(const Vector2& right)
		{
			this->x += right.x;
			this->y += right.y;
			return *this;
		}

		Vector2& Vector2::operator-=(const Vector2& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			return *this;
		}

		Vector2& Vector2::operator*=(const Vector2& right)
		{
			this->x *= right.x;
			this->y *= right.y;
			return *this;
		}

		Vector2& Vector2::operator/=(const Vector2& right)
		{
			this->x /= right.x;
			this->y /= right.y;
			return *this;
		}

		bool Vector2::operator>(Vector2& right)
		{
			return this->Magnitude() > right.Magnitude();
		}

		bool Vector2::operator>=(Vector2& right)
		{
			return this->Magnitude() >= right.Magnitude();
		}

		bool Vector2::operator<(Vector2& right)
		{
			return this->Magnitude() < right.Magnitude();
		}

		bool Vector2::operator<=(Vector2& right)
		{
			return this->Magnitude() <= right.Magnitude();
		}

		bool Vector2::operator==(const Vector2 &right)
		{
			return x == right.x && y == right.y;
		}

		bool Vector2::operator!=(const Vector2 &right)
		{
			return !(*this == right);
		}

		float Vector2::Dot(const Vector2& vector)
		{
			return x * vector.x + y * vector.y;
		}

		float Vector2::Distance(const Vector2& vector)
		{
			Vector2 d = Direction(vector);
			return sqrt((d.x * d.x + d.y * d.y));
		}

		float Vector2::Magnitude()
		{
			return (float)sqrt(Dot(*this));
		}

		Vector2 Vector2::Direction(const Vector2& vector)
		{
			return Vector2(abs(vector.x - x), abs(vector.y - y));
		}

		Vector2 Vector2::Normalize()
		{
			float length = Magnitude();

			if (length > 0)
				return Vector2(x / length, y / length);
			else
				return Vector2(0);
		}

		//VECTOR 3 IMPLEMENTATION
		Vector3::Vector3()
		{
			data = {};
		}
		Vector3::Vector3(const float & v)
		{
			data = { v, v, v };
		}
		Vector3::Vector3(const std::initializer_list<float> args)
		{
			assert(args.size() <= 3);
			int index = 0;
			for (auto begin = args.begin(); begin != args.end(); ++begin) {
				data.at(index++) = *begin;
			}
		}
		Vector3::Vector3(const float & x, const float & y, const float & z)
		{
			data = { x, y, z };
		}
		Vector3::Vector3(const Vector2 & vec, const float & z)
		{
			data = { vec.x, vec.y, z };
		}
		Vector3::Vector3(const float & x, const Vector2 & vec)
		{
			data = { x, vec.x, vec.y };
		}
		
		Vector3 operator+(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
		}

		Vector3 operator-(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
		}

		Vector3 operator*(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
		}

		Vector3 operator/(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
		}

		Vector3& Vector3::operator+=(const Vector3& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			return *this;
		}

		Vector3& Vector3::operator-=(const Vector3& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			return *this;
		}

		Vector3& Vector3::operator*=(const Vector3& right)
		{
			this->x *= right.x;
			this->y *= right.y;
			this->z *= right.z;
			return *this;
		}

		Vector3& Vector3::operator/=(const Vector3& right)
		{
			this->x /= right.x;
			this->y /= right.y;
			this->z /= right.z;
			return *this;
		}

		bool Vector3::operator>(Vector3& right)
		{
			return this->Magnitude() > right.Magnitude();
		}

		bool Vector3::operator>=(Vector3& right)
		{
			return this->Magnitude() >= right.Magnitude();
		}

		bool Vector3::operator<(Vector3& right)
		{
			return this->Magnitude() < right.Magnitude();
		}

		bool Vector3::operator<=(Vector3& right)
		{
			return this->Magnitude() <= right.Magnitude();
		}

		bool Vector3::operator==(const Vector3 &right)
		{
			return x == right.x && y == right.y && z == right.z;
		}

		bool Vector3::operator!=(const Vector3 &right)
		{
			return !(*this == right);
		}



		float Vector3::Dot(const Vector3& vector)
		{
			return x * vector.x + y * vector.y + z * vector.z;
		}

		float Vector3::Distance(const Vector3& vector)
		{
			Vector3 d = Direction(vector);
			return sqrt((d.x * d.x + d.y * d.y + d.z * d.z));
		}

		float Vector3::Magnitude()
		{
			return (float)sqrt(Dot(*this));
		}

		Vector3 Vector3::Cross(const Vector3& vector)
		{
			return Vector3(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
		}

		Vector3 Vector3::Direction(const Vector3& vector)
		{
			return Vector3(abs(vector.x - x), abs(vector.y - y), abs(vector.z - z));
		}

		Vector3 Vector3::Normalize()
		{
			float length = Magnitude();

			if (length > 0)
				return Vector3(x / length, y / length, z / length);
			else
				return Vector3(0);
		}

		//VECTOR 4 IMPLEMENTATION
		Vector4::Vector4()
		{
			data = {};
		}
		Vector4::Vector4(const float & v)
		{
			data = { v, v, v, v };
		}
		Vector4::Vector4(const std::initializer_list<float> args)
		{
			assert(args.size() <= 4);
			int index = 0;
			for (auto begin = args.begin(); begin != args.end(); ++begin) {
				data.at(index++) = *begin;
			}
		}
		Vector4::Vector4(const float & x, const float & y, const float & z, const float & w)
		{
			data = { x, y, z, w };
		}
		Vector4::Vector4(const Vector2 & xy, const Vector2 & zw)
		{
			data = { xy.x, xy.y, zw.x, zw.y };
		}
		Vector4::Vector4(const Vector3 & xyz, const float & w)
		{
			data = { xyz.x, xyz.y, xyz.z, w };
		}
		Vector4::Vector4(const Vector3 & xyz)
		{
			data = { xyz.x, xyz.y, xyz.z, 1 };
		}

		Vector4 operator+(const Vector4& left, const Vector4& right)
		{
			return Vector4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
		}

		Vector4 operator-(const Vector4& left, const Vector4& right)
		{
			return Vector4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
		}

		Vector4 operator*(const Vector4& left, const Vector4& right)
		{
			return Vector4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
		}

		Vector4 operator/(const Vector4& left, const Vector4& right)
		{
			return Vector4(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
		}

		Vector4& Vector4::operator+=(const Vector4& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			this->w += right.w;
			return *this;
		}

		Vector4& Vector4::operator-=(const Vector4& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			this->w -= right.w;
			return *this;
		}

		Vector4& Vector4::operator*=(const Vector4& right)
		{
			this->x *= right.x;
			this->y *= right.y;
			this->z *= right.z;
			this->w *= right.w;
			return *this;
		}

		Vector4& Vector4::operator/=(const Vector4& right)
		{
			this->x /= right.x;
			this->y /= right.y;
			this->z /= right.z;
			this->w /= right.w;
			return *this;
		}

		bool Vector4::operator>(Vector4& right)
		{
			return this->Magnitude() > right.Magnitude();
		}

		bool Vector4::operator>=(Vector4& right)
		{
			return this->Magnitude() >= right.Magnitude();
		}

		bool Vector4::operator<(Vector4& right)
		{
			return this->Magnitude() < right.Magnitude();
		}

		bool Vector4::operator<=(Vector4& right)
		{
			return this->Magnitude() <= right.Magnitude();
		}

		bool Vector4::operator==(const Vector4 &right)
		{
			return x == right.x && y == right.y && z == right.z && w == right.w;
		}

		bool Vector4::operator!=(const Vector4 &right)
		{
			return !(*this == right);
		}
		float Vector4::Dot(const Vector4& vector)
		{
			return x * vector.x + y * vector.y + z * vector.z + w * vector.w;
		}

		float Vector4::Distance(const Vector4& vector)
		{
			Vector4 d = Direction(vector);
			return sqrt((d.x * d.x + d.y * d.y + d.z * d.z + d.w * d.w));
		}

		float Vector4::Magnitude()
		{
			return (float)sqrt(Dot(*this));
		}

		Vector4 Vector4::Direction(const Vector4& vector)
		{
			return Vector4(abs(vector.x - x), abs(vector.y - y), abs(vector.z - z), abs(vector.w - w));
		}

		Vector4 Vector4::Normalize()
		{
			float length = Magnitude();

			if (length > 0)
				return Vector4(x / length, y / length, z / length, w / length);
			else
				return Vector4(0);
		}
	}
}