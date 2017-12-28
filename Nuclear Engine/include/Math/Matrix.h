#pragma once

#include <initializer_list>
#include <assert.h>

#include "Vector.h"

namespace NuclearEngine {

	namespace Math
	{
		/*

		  Generic m by n dimensional Matrix template type version supporting matrices of any type. The
		  Matrix type follows in functionality conventions from the mathematical literature. By default
		  we will only be using floating point matrices, but having a generic version allows us to
		  potentially use double precision matrices as well (or even integer matrices).

		  The matrices are stored in column-major order, the resulting transformations will also assume
		  column-major order, keeping Matrix-Vector multiplications with the Matrix on the left side of
		  the equation and representing Vectors as column Vectors (post-multiplication).

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
					// allow access on a per-column basis. We do not provide support for per-row access 
					// as this is not sequential in memory.
					Vector<m, T> col[n];
				};
			};
			// --------------------------------------------------------------------------------------------
			// consturctor0: default initializes Matrix to identity Matrix 
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
			// --------------------------------------------------------------------------------------------
			// constructor1: initialize Matrix with initializer list
			Matrix(const std::initializer_list<T> args)
			{
				assert(args.size() <= m * n);
				std::size_t cols = 0, rows = 0;

				for (auto& it : args)
				{
					e[cols][rows++] = it;
					if (rows >= m)
					{
						++cols;
						rows = 0;
					}
				}
			}
			// --------------------------------------------------------------------------------------------
			// returns a column Vector, that can again be indexed with the Vector subscript operator. 
			// In effect: [][] and [] indexing is possible.
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


		// per-Matrix operations
		// --------------------------------------------------------------------------------------------
		// addition (note that we do not define Matrix scalar operations as they are not mathematically 
		// defined; they should be defined as  operations on a Matrix completely filled with the 
		// respective scalar.
		template <std::size_t m, std::size_t n, typename T>
		Matrix<m, n, T> operator+(Matrix<m, n, T>& lhs, Matrix<m, n, T>& rhs)
		{
			Matrix<m, n, T> result;
			for (std::size_t col = 0; col < n; ++col)
			{
				for (std::size_t row = 0; row < m; ++row)
				{
					result[col][row] = lhs[col][row] + rhs[col][row];
				}
			}
			return result;
		}
		// subtraction
		// --------------------------------------------------------------------------------------------
		template <std::size_t m, std::size_t n, typename T>
		Matrix<m, n, T> operator-(Matrix<m, n, T>& lhs, Matrix<m, n, T>& rhs)
		{
			Matrix<m, n, T> result;
			for (std::size_t col = 0; col < n; ++col)
			{
				for (std::size_t row = 0; row < m; ++row)
				{
					result[col][row] = lhs[col][row] - rhs[col][row];
				}
			}
			return result;
		}
		// multiplication
		// --------------------------------------------------------------------------------------------
		// note that with Matrix multiplication both matrices can have varying dimensions/sizes as long 
		// as they adhere to the following rule: the number of columns (n) of the LHS Matrix should 
		// equal the number of rows (n) on the RHS Matrix.  Theresult of the Matrix multiplication is 
		// then always a Matrix of dimensions m x o (LHS:rows x RHS:cols) dimensions.
		template <std::size_t m, std::size_t n, std::size_t o, typename T>
		Matrix<m, o, T> operator*(Matrix<m, n, T>& lhs, Matrix<n, o, T>& rhs)
		{
			Matrix<m, o, T> result;
			for (std::size_t col = 0; col < o; ++col)
			{
				for (std::size_t row = 0; row < m; ++row)
				{
					T value = {};
					for (std::size_t j = 0; j < n; ++j) // j equals col in math notation (i = row)
					{
						value += lhs[j][row] * rhs[col][j];
					}
					result[col][row] = value;
				}
			}
			return result;
		}
		// multiplication with reference Matrix (store directly inside provided Matrix)
		// --------------------------------------------------------------------------------------------
		template <std::size_t m, std::size_t n, std::size_t o, typename T>
		Matrix<m, o, T>& mul(Matrix <m, n, T> &result, const Matrix<m, n, T>& lhs, const Matrix<n, o, T>& rhs)
		{
			for (std::size_t col = 0; col < o; ++col)
			{
				for (std::size_t row = 0; row < m; ++row)
				{
					T value = {};
					for (std::size_t j = 0; j < n; ++j) // j equals col in math notation (i = row)
					{
						value += lhs[j][row] * rhs[col][j];
					}
					result[col][row] = value;
				}
			}
			return result;
		}
		// Matrix * Vector multiplication
		// --------------------------------------------------------------------------------------------
		// rhs Vector multiplication. We only define Vector-Matrix multiplication with the Vector on 
		// the right-side of the equation due to the column-major convention.
		template <std::size_t m, std::size_t n, typename T>
		Vector<m, T> operator*(Matrix<m, n, T>& lhs, Vector<n, T>& rhs)
		{
			Vector<m, T> result;
			for (std::size_t row = 0; row < m; ++row)
			{
				T value = {};
				for (std::size_t j = 0; j < n; ++j) // j equals col in math notation (i = row)
				{
					value += lhs[j][row] * rhs[j];
				}
				result[row] = value;
			}
			return result;
		}
	}
}