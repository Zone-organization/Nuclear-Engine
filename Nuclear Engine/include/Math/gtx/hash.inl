/// @ref gtx_hash
/// @file glm/gtx/hash.inl
///
/// @see core (dependence)
///
/// @defgroup gtx_hash GLM_GTX_hash
/// @ingroup gtx
///
/// @brief Add std::hash support for glm types
///
/// <glm/gtx/hash.inl> need to be included to use the features of this extension.

namespace Math {
namespace detail
{
	GLM_INLINE void hash_combine(size_t &seed, size_t hash)
	{
		hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash;
	}
}}

namespace std
{
	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::vec<1, T, Q>>::operator()(Math::vec<1, T, Q> const& v) const
	{
		hash<T> hasher;
		return hasher(v.x);
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::vec<2, T, Q>>::operator()(Math::vec<2, T, Q> const& v) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Math::detail::hash_combine(seed, hasher(v.x));
		Math::detail::hash_combine(seed, hasher(v.y));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::vec<3, T, Q>>::operator()(Math::vec<3, T, Q> const& v) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Math::detail::hash_combine(seed, hasher(v.x));
		Math::detail::hash_combine(seed, hasher(v.y));
		Math::detail::hash_combine(seed, hasher(v.z));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::vec<4, T, Q>>::operator()(Math::vec<4, T, Q> const& v) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Math::detail::hash_combine(seed, hasher(v.x));
		Math::detail::hash_combine(seed, hasher(v.y));
		Math::detail::hash_combine(seed, hasher(v.z));
		Math::detail::hash_combine(seed, hasher(v.w));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::tquat<T, Q>>::operator()(Math::tquat<T,Q> const& q) const
	{
		size_t seed = 0;
		hash<T> hasher;
		Math::detail::hash_combine(seed, hasher(q.x));
		Math::detail::hash_combine(seed, hasher(q.y));
		Math::detail::hash_combine(seed, hasher(q.z));
		Math::detail::hash_combine(seed, hasher(q.w));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::tdualquat<T, Q>>::operator()(Math::tdualquat<T, Q> const& q) const
	{
		size_t seed = 0;
		hash<Math::tquat<T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(q.real));
		Math::detail::hash_combine(seed, hasher(q.dual));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<2, 2, T, Q>>::operator()(Math::mat<2, 2, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<2, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<2, 3, T, Q>>::operator()(Math::mat<2, 3, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<3, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<2, 4, T, Q>>::operator()(Math::mat<2, 4, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<4, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<3, 2, T, Q>>::operator()(Math::mat<3, 2, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<2, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		Math::detail::hash_combine(seed, hasher(m[2]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<3, 3, T, Q>>::operator()(Math::mat<3, 3, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<3, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		Math::detail::hash_combine(seed, hasher(m[2]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<3, 4, T, Q>>::operator()(Math::mat<3, 4, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<4, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		Math::detail::hash_combine(seed, hasher(m[2]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<4, 2, T,Q>>::operator()(Math::mat<4, 2, T,Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<2, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		Math::detail::hash_combine(seed, hasher(m[2]));
		Math::detail::hash_combine(seed, hasher(m[3]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<4, 3, T,Q>>::operator()(Math::mat<4, 3, T,Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<3, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		Math::detail::hash_combine(seed, hasher(m[2]));
		Math::detail::hash_combine(seed, hasher(m[3]));
		return seed;
	}

	template<typename T, Math::qualifier Q>
	GLM_FUNC_QUALIFIER size_t hash<Math::mat<4, 4, T,Q>>::operator()(Math::mat<4, 4, T, Q> const& m) const
	{
		size_t seed = 0;
		hash<Math::vec<4, T, Q>> hasher;
		Math::detail::hash_combine(seed, hasher(m[0]));
		Math::detail::hash_combine(seed, hasher(m[1]));
		Math::detail::hash_combine(seed, hasher(m[2]));
		Math::detail::hash_combine(seed, hasher(m[3]));
		return seed;
	}
}
