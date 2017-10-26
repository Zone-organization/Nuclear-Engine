#pragma once
#include <initializer_list>
#include <assert.h>
#include <NuclearCommon\Math\Vector.h>

#define MATRIX_COLUMN_SIZE 4
#define MATRIX_ROW_SIZE 4
#define MATRIX_SIZE MATRIX_COLUMN_SIZE * MATRIX_ROW_SIZE

namespace NuclearCommon
{
	namespace Math {

		class NCAPI Matrix4
		{
		private:
			float elements[MATRIX_SIZE];

		public:
			Matrix4();
			~Matrix4();

			friend const Matrix4 operator*(const Matrix4& matrixA, const Matrix4& matrixB) { return Matrix4::Multiply(matrixB, matrixA); }

			Matrix4 Identity();
			Matrix4 Translate(const Vector3& translation);
			Matrix4 Scale(const Vector3& scale);
			Matrix4 Rotate(float angle, const Vector3& axis);
			Matrix4 Perspective(float fieldOfView, float aspectRatio, float nearZ, float farZ);
			Matrix4 Orthographic(float left, float right, float top, float bottom, float nearZ, float farZ);
			Matrix4 LookAt(const Vector3 &eye, const Vector3 &target, Vector3 &up);

		private:
			static Matrix4 Multiply(const Matrix4& matrixA, const Matrix4& matrixB);
		};

		
	}
}