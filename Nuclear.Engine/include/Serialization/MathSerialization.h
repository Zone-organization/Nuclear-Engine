#pragma once
#include <Math/Math.h>

namespace glm
{

	//Vectors

	constexpr auto serialize(auto& s, vec2& o) {	return s(o.x,o.y); }


	constexpr auto serialize(auto& s, vec3& o){	return s(o.x,o.y,o.z);	}


	constexpr auto serialize(auto& s, vec4& o){	return s(o.x,o.y,o.z,o.w);	}


	constexpr auto serialize(auto& s, ivec2& o) { return s(o.x,o.y); }


	constexpr auto serialize(auto& s, ivec3& o) { return s(o.x,o.y,o.z); }


	constexpr auto serialize(auto& s, ivec4& o) { return s(o.x,o.y,o.z,o.w); }


	constexpr auto serialize(auto& s, uvec2& o) { return s(o.x,o.y); }


	constexpr auto serialize(auto& s, uvec3& o) { return s(o.x,o.y,o.z); }


	constexpr auto serialize(auto& s, uvec4& o) { return s(o.x,o.y,o.z,o.w); }


	constexpr auto serialize(auto& s, dvec2& o) { return s(o.x,o.y); }


	constexpr auto serialize(auto& s, dvec3& o) { return s(o.x,o.y,o.z); }


	constexpr auto serialize(auto& s, dvec4& o) { return s(o.x,o.y,o.z,o.w); }

	//Quaternions

	constexpr auto serialize(auto& s, quat& o) { return s(o.x,o.y,o.z,o.w); }


	constexpr auto serialize(auto& s, dquat& o) { return s(o.x,o.y,o.z,o.w); }

	//Matrices

	constexpr auto serialize(auto& s, mat2& o) { return s(o[0], o[1]); }


	constexpr auto serialize(auto& s, dmat2& o) { return s(o[0],o[1]); }


	constexpr auto serialize(auto& s, mat3& o) { return s(o[0],o[1],o[2]); }


	constexpr auto serialize(auto& s, dmat3& o) { return s(o[0],o[1],o[2]); }


	constexpr auto serialize(auto& s, mat4& o) { return	s(o[0],o[1],o[2],o[3]);}


	constexpr auto serialize(auto& s, dmat4& o) { return s(o[0],o[1],o[2],o[3]);	}




	constexpr auto serialize(auto& s, const vec2& o) { return s(o.x, o.y); }


	constexpr auto serialize(auto& s, const vec3& o) { return s(o.x, o.y, o.z); }


	constexpr auto serialize(auto& s, const vec4& o) { return s(o.x, o.y, o.z, o.w); }


	constexpr auto serialize(auto& s, const ivec2& o) { return s(o.x, o.y); }


	constexpr auto serialize(auto& s, const ivec3& o) { return s(o.x, o.y, o.z); }


	constexpr auto serialize(auto& s, const ivec4& o) { return s(o.x, o.y, o.z, o.w); }


	constexpr auto serialize(auto& s, const uvec2& o) { return s(o.x, o.y); }


	constexpr auto serialize(auto& s, const uvec3& o) { return s(o.x, o.y, o.z); }


	constexpr auto serialize(auto& s, const uvec4& o) { return s(o.x, o.y, o.z, o.w); }


	constexpr auto serialize(auto& s, const dvec2& o) { return s(o.x, o.y); }


	constexpr auto serialize(auto& s, const dvec3& o) { return s(o.x, o.y, o.z); }


	constexpr auto serialize(auto& s, const dvec4& o) { return s(o.x, o.y, o.z, o.w); }

	//Quaternions

	constexpr auto serialize(auto& s, const quat& o) { return s(o.x, o.y, o.z, o.w); }


	constexpr auto serialize(auto& s, const dquat& o) { return s(o.x, o.y, o.z, o.w); }

	//Matrices

	constexpr auto serialize(auto& s, const mat2& o) { return s(o[0], o[1]); }


	constexpr auto serialize(auto& s, const dmat2& o) { return s(o[0], o[1]); }


	constexpr auto serialize(auto& s, const mat3& o) { return s(o[0], o[1], o[2]); }


	constexpr auto serialize(auto& s, const dmat3& o) { return s(o[0], o[1], o[2]); }


	constexpr auto serialize(auto& s, const mat4& o) { return	s(o[0], o[1], o[2], o[3]); }


	constexpr auto serialize(auto& s, const dmat4& o) { return s(o[0], o[1], o[2], o[3]); }
}