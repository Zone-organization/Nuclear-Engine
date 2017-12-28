#pragma once

#include "Vector.h"
#include "Matrix.h"

// NOTE(Nabil/htmlboss): Going to try to use the built in OpenMP to speed up Matrix operations
#include <omp.h>
namespace NuclearEngine {

	namespace Math
	{
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
		// Note: often we only care about the relative Length differences 
		// between Vectors and not their exact Length values. Seeing as a square 
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

		// Note: Perpendicular is only defined as is for 2D Vectors
		template<typename T>
		inline Vector<2, T> Perpendicular(const Vector<2, T>& vec)
		{
			Vector<2, T> result;
			result.x = -vec.y;
			result.y = vec.x;
			return result;
		}

		// Note: Cross product is only defined for 3D Vectors
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
			for (std::size_t col = 0; col < m; ++col)
			{
				for (std::size_t row = 0; row < n; ++row)
				{
					result[col][row] = mat[row][col];
				}
			}
			return result;
		}

		template <std::size_t  m, std::size_t  n, typename T>
		inline Matrix<m, n, T> Inverse(const Matrix<m, n, T>& mat)
		{
			Matrix<m, n, T> result;
			// TODO(Joey): calculate determinant algebraically and retrieve inverse.
			return result;
		}
	} // namespace Math
}