#pragma once
#include <initializer_list>
#include <assert.h>
#include <array>
#include <NuclearCommon\NuclearCommon.h>

namespace NuclearCommon {
	namespace Math {

		struct NCAPI Vector2
		{
			union
			{
				std::array<float, 2> data;
				// position
				struct
				{
					float x;
					float y;
				};
				// texture coordinates
				struct
				{
					float u;
					float v;
				};
				// texture coordinates
				struct
				{
					float s;
					float t;
				};
			};

			Vector2();
			Vector2(const float& v);
			Vector2(const std::initializer_list<float> args);
			Vector2(const float& x, const float& y);

			float Dot(const Vector2& vector);
			float Distance(const Vector2& vector);
			float Magnitude();
			Vector2 Direction(const Vector2& vector);
			Vector2 Normalize();

			//NOTE: EXPORT
			friend Vector2 NCAPI operator+(const Vector2& left, const Vector2& right);
			friend Vector2 NCAPI operator-(const Vector2& left, const Vector2& right);
			friend Vector2 NCAPI operator*(const Vector2& left, const Vector2& right);
			friend Vector2 NCAPI operator/(const Vector2& left, const Vector2& right);

			Vector2& operator+=(const Vector2& right);
			Vector2& operator-=(const Vector2& right);
			Vector2& operator*=(const Vector2& right);
			Vector2& operator/=(const Vector2& right);

			bool operator>(Vector2& right);
			bool operator>=(Vector2& right);
			bool operator<(Vector2& right);
			bool operator<=(Vector2& right);

			bool operator==(const Vector2& right);
			bool operator!=(const Vector2& right);

			float& operator[] (const std::size_t index)
			{
				assert(index >= 0 && index < 2);
				return data.at(index);
			}
		};
			
		struct NCAPI Vector3
		{
			union
			{
				std::array<float, 3> data;
				// position
				struct
				{
					float x;
					float y;
					float z;
				};
				// color
				struct
				{
					float r;
					float g;
					float b;
				};
				// texture coordinates
				struct
				{
					float s;
					float t;
					float r;
				};
				struct
				{
					Vector2 xy;
				};
				struct
				{
					Vector2 yz;
				};
			};

			Vector3();
			Vector3(const float& v);
			Vector3(const std::initializer_list<float> args);
			Vector3(const float& x, const float& y, const float& z);
			Vector3(const Vector2& vec, const float& z);
			Vector3(const float& x, const Vector2& vec);

			float Dot(const Vector3& vector);
			float Distance(const Vector3& vector);
			float Magnitude();
			Vector3 Cross(const Vector3& vector);
			Vector3 Direction(const Vector3& vector);
			Vector3 Normalize();

			friend NCAPI Vector3 operator+(const Vector3& left, const Vector3& right);
			friend NCAPI Vector3 operator-(const Vector3& left, const Vector3& right);
			friend NCAPI Vector3 operator*(const Vector3& left, const Vector3& right);
			friend NCAPI Vector3 operator/(const Vector3& left, const Vector3& right);

			Vector3& operator+=(const Vector3& right);
			Vector3& operator-=(const Vector3& right);
			Vector3& operator*=(const Vector3& right);
			Vector3& operator/=(const Vector3& right);

			bool operator>(Vector3& right);
			bool operator>=(Vector3& right);
			bool operator<(Vector3& right);
			bool operator<=(Vector3& right);

			bool operator==(const Vector3& right);
			bool operator!=(const Vector3& right);

			float& operator[] (const std::size_t index)
			{
				assert(index >= 0 && index < 3);
				return data.at(index);
			}
		};

		struct NCAPI Vector4
		{
			union
			{
				std::array<float, 4> data;
				// position
				struct
				{
					float x;
					float y;
					float z;
					float w;
				};
				// color
				struct
				{
					float r;
					float g;
					float b;
					float a;
				};
				// texture coordinates
				struct
				{
					float s;
					float t;
					float r;
				};
				struct
				{
					Vector2 xy;
					float _ignored1;
					float _ignored2;
				};
				struct
				{
					float _ignored1;
					float _ignored2;
					Vector2 yz;
				};
				struct
				{
					Vector3 xyz;
					float _ignored1;
				};
				struct
				{
					Vector3 rgb;
					float _ignored1;
				};
				struct
				{
					Vector3 srt;
					float _ignored1;
				};
			};

			Vector4();
			Vector4(const float& v);
			Vector4(const std::initializer_list<float> args);
			Vector4(const float& x, const float& y, const float& z, const float& w = 1.0f);
			Vector4(const Vector2& xy, const Vector2& zw);
			Vector4(const Vector3& xyz, const float& w);
			Vector4(const Vector3& xyz);

			float Dot(const Vector4& vector);
			float Distance(const Vector4& vector);
			float Magnitude();
			Vector4 Direction(const Vector4& vector);
			Vector4 Normalize();

			friend Vector4 NCAPI operator+(const Vector4& left, const Vector4& right);
			friend Vector4 NCAPI operator-(const Vector4& left, const Vector4& right);
			friend Vector4 NCAPI operator*(const Vector4& left, const Vector4& right);
			friend Vector4 NCAPI operator/(const Vector4& left, const Vector4& right);

			Vector4& operator+=(const Vector4& right);
			Vector4& operator-=(const Vector4& right);
			Vector4& operator*=(const Vector4& right);
			Vector4& operator/=(const Vector4& right);

			bool operator>(Vector4& right);
			bool operator>=(Vector4& right);
			bool operator<(Vector4& right);
			bool operator<=(Vector4& right);

			bool operator==(const Vector4& right);
			bool operator!=(const Vector4& right);

			float& operator[] (const std::size_t index)
			{
				assert(index >= 0 && index < 4);
				return data.at(index);
			}
		};
	}
}