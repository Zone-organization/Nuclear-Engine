/// @ref gtx_transform
/// @file glm/gtx/transform.inl

namespace Math
{
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> Translate(vec<3, T, Q> const& v)
	{
		return Translate(mat<4, 4, T, Q>(static_cast<T>(1)), v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> Rotate(T angle, vec<3, T, Q> const& v)
	{
		return Rotate(mat<4, 4, T, Q>(static_cast<T>(1)), angle, v);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> scale(vec<3, T, Q> const& v)
	{
		return scale(mat<4, 4, T, Q>(static_cast<T>(1)), v);
	}

}//namespace glm
