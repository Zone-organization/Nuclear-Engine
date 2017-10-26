#pragma once
#include <Math\Vector.h>
#include <Math\Matrix.h>
#include <Math\Operations.h>

namespace NuclearEngine {
	namespace Math{
			/* NOTE(Joey):

			A Quaternion is a 4-dimensional vector consisting of
			a scalar part w and a 3-dimensional semi-rotation
			axis v: { w, v } or { w, v_x, v_y, v_z }.

			Quaternions are an efficient and precise tool for
			representing rotations in 3D space as an angle-axis
			representation with w being the angle/2 and the
			vector v being the axis of rotation, but normalized
			and scaled by sin(angle/2).

			Note that there is no need for templates here, a
			Quaternion is always a 4-dimensional entity and as
			we always use Quaternions within the range [-1,1]
			float precision is sufficient enough.

			*/
			struct Quaternion
		{
			float w;
			union
			{
				struct
				{
					float x, y, z;
				};
				Math::Vector3 r;
			};

			// NOTE(Joey): construct a zero Quaternion rotation
			Quaternion() : w(0.0f), x(0.0f), y(0.0f), z(0.0f)
			{

			}

			// NOTE(Joey): construct a Quaternion explicitly
			Quaternion(const float w, const float x, const float y, const float z) : w(w), x(x), y(y), z(z)
			{

			}

			// NOTE(Joey): convert from axis-angle format (in radians) to Quaternion
			Quaternion(const Math::Vector3& axis, const float radians)
			{
				const float halfAngle.5f * radians;
				w = cos(halfAngle);
				x = axis.x * sin(halfAngle);
				y = axis.y * sin(halfAngle);
				z = axis.z * sin(halfAngle);
			}
			// NOTE(Joey): convert from axis-angle format (with w being 0.0f)
			explicit Quaternion(const Math::Vector3& axis)
			{
				w.0f;
				x = axis.x;
				y = axis.y;
				z = axis.z;
			}

			Math::Vector4 ToAxisAngle();
			Math::Matrix3 ToMatrix();

			Quaternion operator-();
		};

		// NOTE(Joey): Quaternion algebra
		// ------------------------------
		inline Quaternion Quaternion::operator-()
		{
			return Quaternion(-w, -x, -y, -z);
		}

		inline Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
		{
			return Quaternion(lhs.w + rhs.w, lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		}

		inline Quaternion operator*(const Quaternion& lhs, const float scalar)
		{
			return Quaternion(scalar*lhs.w, scalar*lhs.x, scalar*lhs.y, scalar*lhs.z);
		}
		inline Quaternion operator*(const float scalar, const Quaternion& rhs)
		{
			return Quaternion(scalar*rhs.w, scalar*rhs.x, scalar*rhs.y, scalar*rhs.z);
		}

		// NOTE(Joey): Quaternion geometry
		// -------------------------------
		inline float length(const Quaternion& quat)
		{
			return sqrt(quat.w*quat.w + quat.x*quat.x + quat.y*quat.y + quat.z*quat.z);
		}

		inline Quaternion& normalize(Quaternion& quat)
		{
			const float l = length(quat);
			quat = quat * (1.0f / l);
			return quat;
		}

		inline float dot(const Quaternion& lhs, const Quaternion& rhs)
		{
			return lhs.w*rhs.w + lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
		}

		inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
		{
			Math::Vector3 v1(rhs.x, rhs.y, rhs.z);
			Math::Vector3 v2(lhs.x, lhs.y, lhs.z);

			const float w = rhs.w*lhs.w - dot(rhs, lhs);
			Math::Vector3  v = rhs.w*v2 + lhs.w*v1 + Cross(v2, v1);

			return Quaternion(w, v.x, v.y, v.z);
		}

		// NOTE(Joey): rotates a vector p by Quaternion q. 
		// This rotation equals: q * p * q^-1. To multiply q with p we transform p 
		// to a pure Quaternion (0, p) with a 0 angle and default multiplication.
		// We won't take that exact equation here; instead we use a more efficient 
		// multiplication which is the expanded equation: 
		// (2w^2 - 1)*p + 2*dot(v, p)*r _ 2*w*cross(v, p) 
		// TODO(Joey): this one is important; make sure to test extensively in unit tests!
		inline Math::Vector3 operator*(const Quaternion& quat, const Math::Vector3& vec)
		{
			// TODO(Joey): assert that we're dealing with a unit vector

			const float w2 = quat.w * quat.w;

			return (w2*w2 - 1.0f)*vec + 2.0f*Dot(quat.r, vec)*quat.r + w2*Cross(quat.r, vec);
		}

		// NOTE(Joey): the Quaternion is assumed to be normalized (length of 1)
		inline Quaternion inverse(const Quaternion& quat)
		{
			// TODO(Joey): assert that we're dealing with a unity vector
			return Quaternion(quat.w, -quat.x, -quat.y, -quat.z);
		}

		// NOTE(Joey): conversions
		// -----------------------
		inline Math::Vector4 Quaternion::ToAxisAngle()
		{
			Math::Vector4 result;

			const float angle = 2.0f * acos(w);
			const float length = sqrt(1.0f - angle*angle);

			result.xyz = Math::Vector3(x, y, z) / length;
			result.w = angle;

			return result;
		}

		// NOTE(Joey): as there are a lot of duplicated terms there is a more 
		// efficient way to convert a Quaternion to a matrix; however, to keep the
		// code just a tiny bit more readable (for educational purposes) we perform
		// the convetional math conversion.
		// See'Essential mathematics for games and interactive applications' p.165.
		// NOTTE(Joey): this operation requires the Quaternion to be normalized.
		inline Math::Matrix3 Quaternion::ToMatrix()
		{
			Math::Matrix3 mat;

			mat[0][0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
			mat[0][1] = 2.0f*x*y + 2.0f*w*z;
			mat[0][2] = 2.0f*x*z - 2.0f*w*y;

			mat[1][0] = 2.0f*x*y - 2.0f*w*z;
			mat[1][1] = 1.0f - 2.0f*x*x - 2.0f*z*z;
			mat[1][2] = 2.0f*y*z + 2.0f*w*x;

			mat[2][0] = 2.0f*x*z + 2.0f*w*y;
			mat[2][1] = 2.0f*y*z - 2.0f*w*x;
			mat[2][2] = 1.0f - 2.0f*x*x - 2.0f*y*y;

			return mat;
		}

	}
}