#pragma once
#include "Matrix.h"
#include <algorithm>
// NOTE(Nabil/htmlboss): Going to try to use the built in OpenMP to speed up Matrix operations
#include <omp.h>

#ifdef _WIN32
#define NOMINMAX // <windows.h> has a #define max, min so we need to get rid of that so std::max() and std::min() will work
#endif

const float M_PI = 3.14159265359f;
const float M_TAU = 6.28318530717f;

namespace NuclearEngine
{
	namespace Math {

		template<class T>
		const T& min_(const T& a, const T& b)
		{
			return (b < a) ? b : a;
		}

		template<class T>
		const T& max_(const T& a, const T& b)
		{
			return (a < b) ? b : a;
		}

		// NOTE(To self): when declaring functions in a header file, and multiple 
		// compiled Components include this header file each compilation object will
		// have this function and thus there will be multiple objs with the same
		// function which generates 'already defined in ....obj' errors. Make sure
		// these are inline s.t. this won't occur.
		inline float Deg2Rad(float degrees)
		{
			return degrees / 180.0f * M_PI;
		}
		inline double Deg2Rad(double degrees)
		{
			return degrees / 180.0 * M_PI;
		}

		inline float Rad2Deg(float radians)
		{
			return radians / M_PI * 180.0f;
		}
		inline double Rad2Deg(double radians)
		{
			return radians / M_PI * 180.0;
		}

		// Note: Vector geometric operations
		// ---------------------------------------
		template <std::size_t  n, typename T>
		inline T Length(Vector<n, T> vec)
		{
			T result = {};
			for (std::size_t i = 0; i < n; ++i)
				result += vec[i] * vec[i];
			return sqrt(result);
		}
		// Note: often we only care about the relative length differences 
		// between Vectors and not their exact length values. Seeing as a square 
		// root can be costly, it's more efficient to compare these lengtsh without
		// the square root.
		template <std::size_t n, typename T>
		inline T LengthSquared(Vector<n, T> vec)
		{
			T result = {};
			for (std::size_t i = 0; i < n; ++i)
				result += vec[i] * vec[i];
			return result;
		}

		template <std::size_t n, typename T>
		inline float Distance(Vector<n, T> lhs, Vector<n, T> rhs)
		{
			return Length(lhs - rhs);
		}
		template <std::size_t n, typename T>
		inline float DistanceSquared(Vector<n, T> lhs, Vector<n, T> rhs)
		{
			return LengthSquared(lhs - rhs);
		}

		template <std::size_t n, typename T>
		inline Vector<n, T> Normalize(Vector<n, T> vec)
		{
			Vector<n, T> result;
			T len = Length(vec);
			for (std::size_t i = 0; i < n; ++i)
				result[i] = vec[i] / len;
			return result;
		}

		template <std::size_t  n, typename T>
		inline T Dot(Vector<n, T> lhs, Vector<n, T> rhs)
		{
			T result = {};
			for (std::size_t i = 0; i < n; ++i)
				result += lhs[i] * rhs[i];
			return result;
		}

		// Note: perpendicular is only defined as is for 2D Vectors
		template<typename T>
		inline Vector<2, T> Perpendicular(const Vector<2, T>& vec)
		{
			Vector<2, T> result;
			result.x = -vec.y;
			result.y = vec.x;
			return result;
		}

		// Note: cross product is only defined for 3D Vectors
		template<typename T>
		inline Vector<3, T> Cross(const Vector<3, T>& lhs, const Vector<3, T>& rhs)
		{
			Vector<3, T> result;

			result.x = lhs.y*rhs.z - lhs.z*rhs.y;
			result.y = lhs.z*rhs.x - lhs.x*rhs.z;
			result.z = lhs.x*rhs.y - lhs.y*rhs.x;

			return result;
		}

		// Note: Matrix algebraic operations
		// ---------------------------------------
		template <unsigned int m, unsigned int n, typename T>
		inline Matrix<m, n, T> Transpose(Matrix<m, n, T>& mat)
		{
			Matrix<n, m, T> result;

			// Note: note that we take the rows and cols as nxm instead of mxn this time.
			// We switched the < m and < n around in loop condition (as result Matrix has 
			// reversed dimensions).

			// TODO(Nabil/htmlboss): add in a test for omp parallel
#pragma omp parallel for
			for (std::size_t col; col < m; ++col)
			{
				for (std::size_t row; row < n; ++row)
				{
					result[col][row] = mat[row][col];
				}
			}
			return result;
		}

		template <std::size_t  _m, std::size_t  n, typename T>
		inline Matrix<_m, n, T> Inverse(const Matrix<_m, n, T>& m)
		{

			T Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
			T Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
			T Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

			T Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
			T Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
			T Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

			T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
			T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
			T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

			T Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
			T Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
			T Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

			T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
			T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
			T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

			T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
			T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
			T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

			Vector<4, T> Fac0(Coef00, Coef00, Coef02, Coef03);
			Vector<4, T> Fac1(Coef04, Coef04, Coef06, Coef07);
			Vector<4, T> Fac2(Coef08, Coef08, Coef10, Coef11);
			Vector<4, T> Fac3(Coef12, Coef12, Coef14, Coef15);
			Vector<4, T> Fac4(Coef16, Coef16, Coef18, Coef19);
			Vector<4, T> Fac5(Coef20, Coef20, Coef22, Coef23);

			Vector<4, T> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
			Vector<4, T> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
			Vector<4, T> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
			Vector<4, T> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

			Vector<4, T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
			Vector<4, T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
			Vector<4, T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
			Vector<4, T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

			Vector<4, T> SignA(+1, -1, +1, -1);
			Vector<4, T> SignB(-1, +1, -1, +1);
			Matrix<4, 4, T> inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

			Vector<4, T> Row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

			Vector<4, T> Dot0(m[0] * Row0);
			T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

			T OneOverDeterminant = static_cast<T>(1) / Dot1;

			return inverse * OneOverDeterminant;

			// TODO(Joey): calculate determinant algebraically and retrieve inverse.
			//return result;
		}

		// NOTE(Joey): constants


		// NOTE(Joey): interpolation
		// -------------------------
		template <typename T>
		inline T Lerp(const T& a, const T& b, const float t)
		{
			return (1.0f - t) * a + b * t;
		}

		template <std::size_t n, typename T>
		inline Vector<n, T> Lerp(Vector<n, T>& a, Vector<n, T>& b, const float t)
		{
			Vector<n, T> result;
			for (std::size_t i = 0; i < n; ++i) {
				result[i] = erp(a[i], b[i], t);
			}
			return result;
		}

		// NOTE(Joey): clamp
		// -----------------
		template <typename T>
		inline T Clamp(const T& val, const T& _min, const T& _max)
		{
			return max_(_min, min_(val, _max)); // http://stackoverflow.com/a/9324086/2231969
		}

		template <typename T>
		inline T Clamp01(const T& val)
		{
			return Clamp<T>(val, 0.0f, 1.0f);
		}

		template <std::size_t n, typename T>
		inline T Clamp(const Vector<n, T>& val, const T& min, const T& max)
		{
			Vector<n, T> result;
			for (std::size_t i = 0; i < n; ++i) {
				result[i] = Clamp(val[i], min, max);
			}
			return result;
		}

		template <std::size_t n, typename T>
		inline T Clamp01(const Vector<n, T>& val, const T& min, const T& max)
		{
			Vector<n, T> result;
			for (std::size_t i = 0; i < n; ++i) {
				result[i] = Clamp01(val[i], min, max);
			}
			return result;
		}

		// NOTE(Joey): range (conversion)
		// ------------------------------
		template <typename T>
		inline T NormalizeIntoRange(const T& x, const T& start, const T& end)
		{
			return (x - start) / (end - start);
		}


		// NOTE(Joey): step functions 
		// --------------------------
		template <typename T>
		inline T SmoothStep(const T& e0, const T& e1, const T& x)
		{
			T result;
			result = Clamp01((x - e0) / (e1 - e0));
			result = result * result * (3.0f - 2.0f * result);
			return result;
		}

		template <typename T>
		inline T SmootherStep(const T& e0, const T& e1, const T& x)
		{
			T result;
			result = Clamp01((x - e0) / (e1 - e0));
			result = (result * result * result) * (result * (result * 6.0f - 15.0f) + 10.0f);
			return result;
		}

	}
}