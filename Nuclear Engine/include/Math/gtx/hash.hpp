/// @ref gtx_hash
/// @file glm/gtx/hash.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_hash GLM_GTX_hash
/// @ingroup gtx
///
/// Include <glm/gtx/hash.hpp> to use the features of this extension.
/// 
/// Add std::hash support for glm types

#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_hash is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#include <functional>

#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../gtc/vec1.hpp"

#include "../gtc/quaternion.hpp"
#include "../gtx/dual_quaternion.hpp"

#include "../mat2x2.hpp"
#include "../mat2x3.hpp"
#include "../mat2x4.hpp"

#include "../mat3x2.hpp"
#include "../mat3x3.hpp"
#include "../mat3x4.hpp"

#include "../mat4x2.hpp"
#include "../mat4x3.hpp"
#include "../mat4x4.hpp"

#if !GLM_HAS_CXX11_STL
#	error "GLM_GTX_hash requires C++11 standard library support"
#endif

namespace std
{
	template<typename T, Math::qualifier Q>
	struct hash<Math::vec<1, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::vec<1, T, Q> const& v) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::vec<2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::vec<2, T, Q> const& v) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::vec<3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::vec<3, T, Q> const& v) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::vec<4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::vec<4, T, Q> const& v) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::tquat<T,Q>>
	{
		GLM_FUNC_DECL size_t operator()(Math::tquat<T, Q> const& q) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::tdualquat<T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::tdualquat<T,Q> const& q) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<2, 2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<2, 2, T,Q> const& m) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<2, 3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<2, 3, T,Q> const& m) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<2, 4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<2, 4, T,Q> const& m) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<3, 2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<3, 2, T,Q> const& m) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<3, 3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<3, 3, T,Q> const& m) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<3, 4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<3, 4, T,Q> const& m) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<4, 2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<4, 2, T,Q> const& m) const;
	};
	
	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<4, 3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<4, 3, T,Q> const& m) const;
	};

	template<typename T, Math::qualifier Q>
	struct hash<Math::mat<4, 4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(Math::mat<4, 4, T,Q> const& m) const;
	};
} // namespace std

#include "hash.inl"
