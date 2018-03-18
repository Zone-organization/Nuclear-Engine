/// @ref gtx_mixed_product
/// @file glm/gtx/mixed_product.inl

namespace Math
{
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T mixedProduct
	(
		vec<3, T, Q> const& v1,
		vec<3, T, Q> const& v2,
		vec<3, T, Q> const& v3
	)
	{
		return dot(Cross(v1, v2), v3);
	}
}//namespace glm