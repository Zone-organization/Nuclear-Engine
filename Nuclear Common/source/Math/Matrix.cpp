#include "..\..\include\NuclearCommon\Math\Matrix.h"
#define _USE_MATH_DEFINES
#include <math.h>
namespace NuclearCommon
{
	namespace Math {
		/*
		This matrix class uses column major memory layout with column major vectors.
		This class assums you will do your matrix and vector multiplication in this order.
		M = Iv
		Where M is the new matrix. I is the identity matrix. v is the vector matrix.
		This will match both DirectX and OpenGL column major memory layout and will use column major vector in the shader.

		This matrix class also uses a left handed coordinates system.

		index         value                 layout
		0  4  8  12   M11  M12  M13  M14    Xx Yx Zz Tx
		1  5  9  13   M21  M22  M23  M24    Xy Yy Zy Ty
		2  6  10 14   M31  M32  M33  M34    Xz Yz Zz Tz
		3  7  11 15   M41  M42  M43  M44    0  0  0  1

		C++ array indices		[0,   1,   2,   3  ], [4,   5,   6,   7  ], [8,   9,   10,  11 ]  [12,  13,  14,  15 ]
		Column major indexing	[M11, M21, M31, M41], [M12, M22, M32, M42], [M13, M23, M33, M43]  [M14, M24, M34, M44]
		*/

		float ToRadians(float Degrees)
		{
			return Degrees * (float)(M_PI / 180.0f);
		}

		Matrix4::Matrix4()
		{
			memset(elements, 0, sizeof(float) * MATRIX_SIZE);
		}

		Matrix4::~Matrix4()
		{
		}

		Matrix4 Matrix4::Identity()
		{
			Matrix4 identityMatrix;

			identityMatrix.elements[0 + 0 * MATRIX_COLUMN_SIZE] = 1.0;
			identityMatrix.elements[1 + 1 * MATRIX_COLUMN_SIZE] = 1.0;
			identityMatrix.elements[2 + 2 * MATRIX_COLUMN_SIZE] = 1.0;
			identityMatrix.elements[3 + 3 * MATRIX_COLUMN_SIZE] = 1.0;

			return identityMatrix;
		}

		Matrix4 Matrix4::Translate(const Vector3& translation)
		{
			Matrix4 translateMatrix = Matrix4::Identity();

			translateMatrix.elements[0 + 3 * MATRIX_COLUMN_SIZE] = translation.x;
			translateMatrix.elements[1 + 3 * MATRIX_COLUMN_SIZE] = translation.y;
			translateMatrix.elements[2 + 3 * MATRIX_COLUMN_SIZE] = translation.z;

			return translateMatrix;
		}

		Matrix4 Matrix4::Scale(const Vector3& scale)
		{
			Matrix4 scaleMatrix = Matrix4::Identity();

			scaleMatrix.elements[0 + 0 * MATRIX_COLUMN_SIZE] = scale.x;
			scaleMatrix.elements[1 + 1 * MATRIX_COLUMN_SIZE] = scale.y;
			scaleMatrix.elements[2 + 2 * MATRIX_COLUMN_SIZE] = scale.z;

			return scaleMatrix;
		}

		Matrix4 Matrix4::Rotate(float angle, const Vector3& axis)
		{
			Matrix4 rotationMatrix = Matrix4::Identity();

			float r = ToRadians(angle);
			float c = (float)cos(r);
			float s = (float)sin(r);
			float oc = 1.0f - c;

			rotationMatrix.elements[0 + 0 * MATRIX_COLUMN_SIZE] = axis.x * axis.x * oc + c;
			rotationMatrix.elements[1 + 0 * MATRIX_COLUMN_SIZE] = axis.x * axis.y * oc + axis.z * s;
			rotationMatrix.elements[2 + 0 * MATRIX_COLUMN_SIZE] = axis.x * axis.z * oc - axis.y * s;

			rotationMatrix.elements[0 + 1 * MATRIX_COLUMN_SIZE] = axis.x * axis.y * oc - axis.z * s;
			rotationMatrix.elements[1 + 1 * MATRIX_COLUMN_SIZE] = axis.y * axis.y * oc + c;
			rotationMatrix.elements[2 + 1 * MATRIX_COLUMN_SIZE] = axis.y * axis.z * oc + axis.x * s;

			rotationMatrix.elements[0 + 2 * MATRIX_COLUMN_SIZE] = axis.x * axis.z * oc + axis.y * s;
			rotationMatrix.elements[1 + 2 * MATRIX_COLUMN_SIZE] = axis.y * axis.z * oc - axis.x * s;
			rotationMatrix.elements[2 + 2 * MATRIX_COLUMN_SIZE] = axis.z * axis.z * oc + c;

			return rotationMatrix;
		}

		Matrix4 Matrix4::Perspective(float fieldOfView, float aspectRatio, float nearZ, float farZ)
		{
			Matrix4 perspectiveMatrix;

			float t = (float)tan(ToRadians(fieldOfView * 0.5f));

			perspectiveMatrix.elements[0 + 0 * MATRIX_COLUMN_SIZE] = 1.0f / (t * aspectRatio);
			perspectiveMatrix.elements[1 + 1 * MATRIX_COLUMN_SIZE] = 1.0f / t;
			perspectiveMatrix.elements[2 + 2 * MATRIX_COLUMN_SIZE] = (-nearZ - farZ) / (nearZ - farZ);
			perspectiveMatrix.elements[3 + 2 * MATRIX_COLUMN_SIZE] = 1.0f;
			perspectiveMatrix.elements[2 + 3 * MATRIX_COLUMN_SIZE] = (2.0f * farZ * nearZ) / (nearZ - farZ);

			return perspectiveMatrix;
		}

		Matrix4 Matrix4::Orthographic(float left, float right, float top, float bottom, float nearZ, float farZ)
		{
			Matrix4 orthographicMatrix = Identity();

			orthographicMatrix.elements[0 + 0 * MATRIX_COLUMN_SIZE] = 2.0f / (right - left);
			orthographicMatrix.elements[1 + 1 * MATRIX_COLUMN_SIZE] = 2.0f / (top - bottom);
			orthographicMatrix.elements[2 + 2 * MATRIX_COLUMN_SIZE] = 2.0f / (nearZ - farZ);
			orthographicMatrix.elements[0 + 3 * MATRIX_COLUMN_SIZE] = (left + right) / (left - right);
			orthographicMatrix.elements[1 + 3 * MATRIX_COLUMN_SIZE] = (bottom + top) / (bottom - top);
			orthographicMatrix.elements[2 + 3 * MATRIX_COLUMN_SIZE] = (farZ + nearZ) / (farZ - nearZ);

			return orthographicMatrix;
		}

		Matrix4 Matrix4::LookAt(const Vector3 &eye, const Vector3 &target,  Vector3 &upVector)
		{
			Matrix4 viewMatrix = Matrix4::Identity();

			Vector3 forward = (target - eye).Normalize();
			Vector3 right = upVector.Cross(forward).Normalize();
			Vector3 up = forward.Cross(right);

			viewMatrix.elements[0 + 0 * MATRIX_COLUMN_SIZE] = right.x;
			viewMatrix.elements[0 + 1 * MATRIX_COLUMN_SIZE] = right.y;
			viewMatrix.elements[0 + 2 * MATRIX_COLUMN_SIZE] = right.z;

			viewMatrix.elements[1 + 0 * MATRIX_COLUMN_SIZE] = up.x;
			viewMatrix.elements[1 + 1 * MATRIX_COLUMN_SIZE] = up.y;
			viewMatrix.elements[1 + 2 * MATRIX_COLUMN_SIZE] = up.z;

			viewMatrix.elements[2 + 0 * MATRIX_COLUMN_SIZE] = forward.x;
			viewMatrix.elements[2 + 1 * MATRIX_COLUMN_SIZE] = forward.y;
			viewMatrix.elements[2 + 2 * MATRIX_COLUMN_SIZE] = forward.z;

			viewMatrix.elements[0 + 3 * MATRIX_COLUMN_SIZE] = -right.Dot(eye);
			viewMatrix.elements[1 + 3 * MATRIX_COLUMN_SIZE] = -up.Dot(eye);
			viewMatrix.elements[2 + 3 * MATRIX_COLUMN_SIZE] = -forward.Dot(eye);

			return viewMatrix;
		}

		Matrix4 Matrix4::Multiply(const Matrix4& matrixA, const Matrix4& matrixB)
		{
			Matrix4 newMatrix;

			for (unsigned int row = 0; row < MATRIX_ROW_SIZE; row++)
			{
				for (unsigned int col = 0; col < MATRIX_COLUMN_SIZE; col++)
				{
					for (unsigned int e = 0; e < MATRIX_ROW_SIZE; e++)
						newMatrix.elements[e + row * MATRIX_ROW_SIZE] += matrixA.elements[col + row * MATRIX_COLUMN_SIZE] * matrixB.elements[e + col * MATRIX_COLUMN_SIZE];
				}
			}

			return newMatrix;
		}
	}
}