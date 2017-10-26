#pragma once
#include <initializer_list>
#include <assert.h>

#include "Vector.h"

namespace NuclearEngine
{
	namespace Math {


		/* NOTE(Joey):

		Generic m by n dimensional Matrix template type version supporting matrices
		of any type. The Matrix type follows in functionality conventions from the
		mathematical literature. By default we will only be using floating point
		matrices, but having a generic version allows us to potentially use
		double precision matrices as well (or even integer matrices).

		The matrices are stored in column-major order, the resulting transformations
		will also assume column-major order, keeping Matrix-Vector multiplications
		with the Matrix on the left side of the equation and representing Vectors
		as column Vectors (post-multiplication).

		Matrix numbering by math conventions:
		|  0  1  2  3 |
		|  4  5  6  7 |
		|  8  9 10 11 |
		| 12 13 14 15 |
		Column-major layout in memory:
		[ 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 ]
		Matrix numbering if we access column-major memory sequentially in the array:
		|  0  4  8 12 |
		|  1  5  9 13 |
		|  2  6 10 14 |
		|  3  7 11 15 |

		There is no need for Matrix template specialization.

		*/
		template <std::size_t m, std::size_t n, typename T>
		struct Matrix
		{
			union
			{
				T e[n][m];
				struct
				{
					// NOTE(Joey): allow access on a per-column basis.
					// We do not provide support for per-row access as this is not
					// sequential in memory.
					Vector<m, T> col[n];
				};
			};

			// NOTE(Joey): consturctor0: default initializes Matrix to identity Matrix 
			Matrix()
			{
				for (std::size_t col = 0; col < n; ++col)
				{
					for (std::size_t row = 0; row < m; ++row)
					{
						e[col][row] = (col == row) ? T(1.0f) : T(0.0f);
					}
				}
			}

			// NOTE(Joey): constructor1: initialize Matrix with initializer list
			Matrix(const std::initializer_list<T> args)
			{
				assert(args.size() <= m * n);
				std::size_t cols, rows;

				for (auto& it : args)
				{
					e[cols][rows++] = it;
					if (rows >= m)
					{
						++cols;
						rows;
					}
				}
			}

			// NOTE(Joey): returns a column Vector, that can again be indexed with the Vector subscript 
			// operator. In effect: [][] and [] indexing is possible.
			Vector<m, T>& operator[](const std::size_t colIndex)
			{
				assert(colIndex >= 0 && colIndex < n);
				return col[colIndex];
			}
		};

		typedef Matrix<2, 2, float>  Matrix2;
		typedef Matrix<3, 3, float>  Matrix3;
		typedef Matrix<4, 4, float>  Matrix4;
		typedef Matrix<2, 2, double> Matrix2d;
		typedef Matrix<3, 3, double> Matrix3d;
		typedef Matrix<4, 4, double> Matrix4d;


		// NOTE(Joey): per-Matrix operations
		// ---------------------------------
		// NOTE(Joey): addition (note that we do not define Matrix scalar operations
		// as they are not mathematically defined; they should be defined as 
		// operations on a Matrix completely filled with the respective scalar.
		// TODO(Joey): consider using pointers/references instead of using the
		// stack for passing arguments here as the memory savings might now 
		// outweigh the cost of walking the pointers.
		template <std::size_t m, std::size_t n, typename T>
		Matrix<m, n, T> operator+(Matrix<m, n, T>& lhs, Matrix<m, n, T>& rhs)
		{
			Matrix<m, n, T> result;
			for (std::size_t col; col < n; ++col)
			{
				for (std::size_t row; row < m; ++row)
				{
					result[col][row] = lhs[col][row] + rhs[col][row];
				}
			}
			return result;
		}
		// NOTE(Joey): subtraction
		template <std::size_t m, std::size_t n, typename T>
		Matrix<m, n, T> operator-(Matrix<m, n, T>& lhs, Matrix<m, n, T>& rhs)
		{
			Matrix<m, n, T> result;
			for (std::size_t col; col < n; ++col)
			{
				for (std::size_t row; row < m; ++row)
				{
					result[col][row] = lhs[col][row] - rhs[col][row];
				}
			}
			return result;
		}
		// NOTE(Joey): multiplication
		// NOTE(Joey): note that with Matrix multiplication both matrices can have
		// varying dimensions/sizes as long as they adhere to the following rule:
		// - The number of columns (n) of the LHS Matrix should equal the number
		//   of rows (n) on the RHS Matrix.
		// The result of the Matrix multiplication is then always a Matrix of 
		// dimensions m x o (LHS:rows x RHS:cols) dimensions.
		template <std::size_t m, std::size_t n, std::size_t o, typename T>
		Matrix<m, o, T> operator*(Matrix<m, n, T>& lhs, Matrix<n, o, T>& rhs)
		{
			Matrix<m, o, T> result;
			for (std::size_t col = 0; col < o; ++col)
			{
				for (std::size_t row = 0; row < m; ++row)
				{
					T value = {};
					for (std::size_t j = 0; j < n; ++j) // NOTE(Joey): j equals col in math notation (i = row)
					{
						value += lhs[j][row] * rhs[col][j];
					}
					result[col][row] = value;
				}
			}
			return result;
		}

		// NOTE(Joey): multiplication with reference Matrix (store directly inside provided Matrix)
		template <std::size_t m, std::size_t n, std::size_t o, typename T>
		Matrix<m, o, T>& mul(Matrix <m, n, T> &result, const Matrix<m, n, T>& lhs, const Matrix<n, o, T>& rhs)
		{
			for (std::size_t col; col < o; ++col)
			{
				for (std::size_t row; row < m; ++row)
				{
					T value = {};
					for (std::size_t j; j < n; ++j) // NOTE(Joey): j equals col in math notation (i = row)
					{
						value += lhs[j][row] * rhs[col][j];
					}
					result[col][row] = value;
				}
			}
			return result;
		}

		// NOTE(Joey): Vector-Matrix operations
		// ------------------------------------
		// NOTE(Joey): rhs Vector multiplication. We only define Vector-Matrix
		// multiplication with the Vector on the right-side of the equation due
		// to the column-major convention.
		template <std::size_t m, std::size_t n, typename T>
		Vector<m, T> operator*(Matrix<m, n, T>& lhs, Vector<n, T>& rhs)
		{
			Vector<m, T> result;
			for (std::size_t row; row < m; ++row)
			{
				T value = {};
				for (std::size_t j; j < n; ++j) // NOTE(Joey): j equals col in math notation (i = row)
				{
					value += lhs[j][row] * rhs[j];
				}
				result[row] = value;
			}
			return result;
		}

		// NOTE(Joey): scale
		// -----------------
		template <std::size_t n, typename T>
		Matrix<n, n, T> Scale(Vector<n, T>& scale)
		{
			Matrix<n, n, T> mat;
			for (std::size_t i = 0; i < n; ++i) {
				mat[i][i] = scale[i];
			}
			return mat;
		}
		// NOTE(Joey): version w/ reference
		template <std::size_t n, typename T>
		Matrix<n, n, T>& Scale(Matrix<n, n, T>& result, Vector<n, T> scale)
		{
			// NOTE(Joey): we can do a manual operation on the Matrix scale
			for (std::size_t i = 0; i < n; ++i) {
				result[i][i] *= scale[i];
			}
			return result;
		}

		// NOTE(Joey): specialization on affine matrices with Matrix and scale
		// Vector having different dimensions
		template <typename T>
		Matrix<4, 4, T> Scale(Vector<3, T> scale)
		{
			Matrix<4, 4, T> mat;
			for (std::size_t i = 0; i < 3; ++i) {
				mat[i][i] = scale[i];
			}
			return mat;
		}
		// NOTE(Joey): version w/ reference
		template <typename T>
		Matrix<4, 4, T>& Scale(Matrix<4, 4, T>& result, Vector<3, T>& scale)
		{
			// NOTE(Joey): we can do a manual operation on the Matrix scale
			for (std::size_t i = 0; i < 3; ++i) {
				result[i][i] *= scale[i];
			}
			return result;
		}

		// NOTE(Joey): rotation
		// --------------------
		// NOTE(Joey): getting a direct rotation Matrix is only allowed on 3 dimensions
		template <typename T>
		Matrix<4, 4, T> Rotate(const Vector<3, T>& axis, const T& angle)
		{
			Matrix<4, 4, T> result;

			const float c = cos(angle);
			const float s = sin(angle);
			const float t = 1.0f - c;

			result[0][0] = t*axis.x*axis.x + c;
			result[0][1] = t*axis.x*axis.y + s*axis.z;
			result[0][2] = t*axis.x*axis.z - s*axis.y;

			result[1][0] = t*axis.x*axis.y - s*axis.z;
			result[1][1] = t*axis.y*axis.y + c;
			result[1][2] = t*axis.y*axis.z + s*axis.x;

			result[2][0] = t*axis.x*axis.z + s*axis.y;
			result[2][1] = t*axis.y*axis.z - s*axis.x;
			result[2][2] = t*axis.z*axis.z + c;

			return result;
		}

		template <typename T>
		Matrix<4, 4, T>& Rotate(Matrix<4, 4, T> &result, const Vector<3, T>& axis, const T& angle)
		{
			Matrix<4, 4, T> rot = Rotate(axis, angle);
			result = result * rot;
			return result;
		}


		// NOTE(Joey): translation
		// -----------------------
		// NOTE(Joey): translations are only defined for 4-dimensional matrices/Vectors
		template <typename T>
		Matrix<4, 4, T> Translate(const Vector<3, T>& translation)
		{
			Matrix<4, 4, T> mat;
			mat[3] = Vector<4, T>(translation, 1.0);
			return mat;
		}

		template <typename T>
		Matrix<4, 4, T>& Translate(Matrix<4, 4, T> &result, const Vector<3, T>& translation)
		{
			Matrix<4, 4, T> mat;
			mat[3] = Vector<4, T>(translation, 1.0);

			result = result * mat;
			return result;
		}

		// NOTE(Joey): view-space 
		// ----------------------
		template <typename T>
		Matrix<4, 4, T> LookAt(Vector<3, T>& eye, Vector<3, T>& center, Vector<3, T>& up)
		{
			//Matrix<4, 4, T> frame;
			//Matrix<4, 4, T> translate;

			//Math::Vector3 forward = Normalize(target - position);
			//Math::Vector3 right = Normalize(Cross(forward, worldUp));
			//Math::Vector3 up = Cross(right, forward);

			//frame[0].xyz = right;
			//frame[1].xyz = up;
			//// NOTE(Joey): negative forward as we're looking towards negative z-axis
			//frame[2].xyz = -forward;
			//// NOTE(Joey): we want the inverse of the rotation part, which is equal
			//// to its transpose as the Matrix's column Vectors represent a 
			//// orthogonal basis.
			//frame = Math::Transpose(frame);

			//// NOTE(Joey): the inverse of the translation Matrix is just its 
			//// translation Vector negated.
			//translate[3].xyz = -position;

			//return frame*translate;

			Math::Vector3 const f(Normalize(center - eye));
			Math::Vector3 const s(Normalize(Cross(f, up)));
			Math::Vector3 const u(Cross(s, f));

			Matrix<4, 4, T> Result;
			Result[0][0] = s.x;
			Result[1][0] = s.y;
			Result[2][0] = s.z;
			Result[0][1] = u.x;
			Result[1][1] = u.y;
			Result[2][1] = u.z;
			Result[0][2] = -f.x;
			Result[1][2] = -f.y;
			Result[2][2] = -f.z;
			Result[3][0] = -Dot(s, eye);
			Result[3][1] = -Dot(u, eye);
			Result[3][2] = Dot(f, eye);
			return Result;
		}

		// NOTE(Joey): projection
		// ----------------------
		template <typename T>
		Matrix<4, 4, T> Orthographic(const T& left, const T& right, const T& top, const T& bottom, const T& _near, const T& _far)
		{
			Matrix<4, 4, T> result;

			result[0][0] = 2.0f / (right - left);

			result[1][1] = 2.0f / (top - bottom);

			result[2][2] = -2.0f / (_far - _near);

			result[3][0] = -(right + left) / (right - left);
			result[3][1] = -(top + bottom) / (top - bottom);
			result[3][2] = -(_far + _near) / (_far - _near);
			result[3][3] = 1.0f;

			return result;
		}

		template <typename T>
		Matrix<4, 4, T> Perspective(const T& fov, const T& aspect, const T& _near, const T& _far)
		{
			Matrix<4, 4, T> Result;

			//float top = _near * tan(fov / 2.0);
			//float bottom = -top;
			//float right = top * aspect;
			//float left = -top * aspect;

			//result[0][0] = (2.0f*_near) / (right - left);

			//result[1][1] = (2.0f*_near) / (top - bottom);

			////result[2][0] = (right + left) / (right - left);
			////result[2][1] = (top + bottom) / (top - bottom);
			//result[2][2] = -(_far + _near) / (_far - _near); // NOTE(Joey): same as: (n + f) / (n - f)
			//result[2][3] = -1.0f;

			//result[3][2] = -(2.0f*_near*_far) / (_far - _near); // NOTE(Joey): same as 2nf / (n-f)
			//result[3][3].0f;

			assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

			T const tanHalfFovy = tan(fov / static_cast<T>(2));

			Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
			Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
			Result[2][2] = -(_far + _near) / (_far - _near);
			Result[2][3] = -static_cast<T>(1);
			Result[3][2] = -(static_cast<T>(2) * _far * _near) / (_far - _near);
			return Result;
		}
	}
}