#pragma once
#include <Base\Math\detail\type_vec.hpp>
#include <Base\Math\detail\type_mat.hpp>
#include <Base/Math/gtc/quaternion.hpp>

namespace glm 
{
		typedef vec2 Vector2;
		typedef vec3 Vector3;
		typedef vec4 Vector4;

		typedef dvec2 Vector2d;
		typedef dvec3 Vector3d;
		typedef dvec4 Vector4d;

		typedef ivec2 Vector2i;
		typedef ivec3 Vector3i;
		typedef ivec4 Vector4i;

		typedef uvec2 Vector2ui;
		typedef uvec3 Vector3ui;
		typedef uvec4 Vector4ui;

		typedef mat2 Matrix2;
		typedef mat3 Matrix3;
		typedef mat4 Matrix4;

		typedef quat Quaternion;
}