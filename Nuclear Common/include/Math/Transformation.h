#pragma once
#include <assert.h>

#include "Vector.h"
#include "Matrix.h"

namespace Math
{
    // Note: scale
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
    // Note: version w/ reference
    template <std::size_t n, typename T>
    Matrix<n, n, T>& Scale(Matrix<n, n, T>& result, Vector<n, T> scale)
    {
        // Note: we can do a manual operation on the Matrix scale
		for (std::size_t i = 0; i < n; ++i) {
			result[i][i] *= scale[i];
		}
        return result;
    }

    // Note: specialization on affine matrices with Matrix and scale
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
    // Note: version w/ reference
    template <typename T>
    Matrix<4, 4, T>& Scale(Matrix<4, 4, T>& result, Vector<3, T>& scale)
    {
        // Note: we can do a manual operation on the Matrix scale
		for (std::size_t i = 0; i < 3; ++i) {
			result[i][i] *= scale[i];
		}
        return result;
    }

    // Note: rotation
    // --------------------
    // Note: getting a direct rotation Matrix is only allowed on 3 dimensions
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


    // Note: translation
    // -----------------------
    // Note: translations are only defined for 4-dimensional matrices/Vectors
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

    // Note: view-space 
    // ----------------------
    template <typename T>
    Matrix<4, 4, T> LookAt(Vector<3, T>& position, Vector<3, T>& target, Vector<3, T>& worldUp)
    {
        Matrix<4, 4, T> frame;
        Matrix<4, 4, T> translate;

        Vector3 forward = Normalize(target - position);
        Vector3 right   = Normalize(Cross(forward, worldUp));
        Vector3 up      = Cross(right, forward);

        frame[0].xyz = right;
        frame[1].xyz = up;
        // Note: negative forward as we're looking towards negative z-axis
        frame[2].xyz = -forward; 
        // Note: we want the inverse of the rotation part, which is equal
        // to its transpose as the Matrix's column Vectors represent a 
        // orthogonal basis.
        frame = Transpose(frame);

        // Note: the inverse of the translation Matrix is just its 
        // translation Vector negated.
        translate[3].xyz = -position;

        return frame*translate;
    }

    // Note: projection
    // ----------------------
    template <typename T>
    Matrix<4, 4, T> Orthographic(T left, T right, T top, T bottom, T near_plane, T far_plane)
    {
        Matrix<4, 4, T> result;

        result[0][0] = 2.0f / (right - left);
        
        result[1][1] = 2.0f / (top - bottom);

        result[2][2] = -2.0f / (far_plane - near_plane);

        result[3][0] = -(right + left) / (right - left);
        result[3][1] = -(top + bottom) / (top - bottom);
        result[3][2] = -(far_plane + near_plane) / (far_plane - near_plane);
        result[3][3] = 1.0f;

        return result;
    }

    template <typename T>
    Matrix<4, 4, T> Perspective(T fov, T aspect, T near_plane, T far_plane)
    {
        Matrix<4, 4, T> result;

        float top    = near_plane * tan(fov / 2.0);
        float bottom = -top;
        float right  =  top * aspect;
        float left   = -top * aspect;

        result[0][0] = (2.0f*near_plane) / (right - left);

        result[1][1] = (2.0f*near_plane) / (top - bottom);

        //result[2][0] = (right + left) / (right - left);
        //result[2][1] = (top + bottom) / (top - bottom);
        result[2][2] = -(far_plane + near_plane) / (far_plane - near_plane); // Note: same as: (n + f) / (n - f)
        result[2][3] = -1.0f;

        result[3][2] = -(2.0f*near_plane*far_plane) / (far_plane - near_plane); // Note: same as 2nf / (n-f)
        result[3][3] = 0.0f;
        

        return result;
    }
} // namespace Math
