#pragma once
#include <Math/Math.h>

namespace glm
{

	//Vectors
	template<typename S>
	void serialize(S& s, vec2& o) {	s.value4b(o.x);	s.value4b(o.y); }

	template<typename S>
	void serialize(S& s, vec3& o){	s.value4b(o.x);	s.value4b(o.y);	s.value4b(o.z);	}

	template<typename S>
	void serialize(S& s, vec4& o){	s.value4b(o.x);	s.value4b(o.y);	s.value4b(o.z);	s.value4b(o.w);	}

	template<typename S>
	void serialize(S& s, ivec2& o) { s.value4b(o.x); s.value4b(o.y); }

	template<typename S>
	void serialize(S& s, ivec3& o) { s.value4b(o.x); s.value4b(o.y); s.value4b(o.z); }

	template<typename S>
	void serialize(S& s, ivec4& o) { s.value4b(o.x); s.value4b(o.y); s.value4b(o.z);	s.value4b(o.w); }

	template<typename S>
	void serialize(S& s, uvec2& o) { s.value4b(o.x); s.value4b(o.y); }

	template<typename S>
	void serialize(S& s, uvec3& o) { s.value4b(o.x); s.value4b(o.y); s.value4b(o.z); }

	template<typename S>
	void serialize(S& s, uvec4& o) { s.value4b(o.x); s.value4b(o.y); s.value4b(o.z); s.value4b(o.w); }

	template<typename S>
	void serialize(S& s, dvec2& o) { s.value8b(o.x); s.value8b(o.y); }

	template<typename S>
	void serialize(S& s, dvec3& o) { s.value8b(o.x); s.value8b(o.y); s.value8b(o.z); }

	template<typename S>
	void serialize(S& s, dvec4& o) { s.value8b(o.x); s.value8b(o.y); s.value8b(o.z); s.value8b(o.w); }

	//Quaternions
	template<typename S>
	void serialize(S& s, quat& o) { s.value4b(o.x);	s.value4b(o.y);	s.value4b(o.z);	s.value4b(o.w); }

	template<typename S>
	void serialize(S& s, dquat& o) { s.value8b(o.x); s.value8b(o.y); s.value8b(o.z); s.value8b(o.w); }

	//Matrices
	template<typename S>
	void serialize(S& s, mat2& o) { s.object(o[0]);	s.object(o[1]); }

	template<typename S>
	void serialize(S& s, dmat2& o) { s.object(o[0]); s.object(o[1]); }

	template<typename S>
	void serialize(S& s, mat3& o) { s.object(o[0]); s.object(o[1]); s.object(o[2]); }

	template<typename S>
	void serialize(S& s, dmat3& o) { s.object(o[0]); s.object(o[1]); s.object(o[2]); }

	template<typename S>
	void serialize(S& s, mat4& o) {	s.object(o[0]); s.object(o[1]); s.object(o[2]); s.object(o[3]);}

	template<typename S>
	void serialize(S& s, dmat4& o) {s.object(o[0]); s.object(o[1]); s.object(o[2]); s.object(o[3]);	}
}