/// @ref gtx_perpendicular
/// @file glm/gtx/perpendicular.inl

namespace Math
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType perp(genType const& x, genType const& Normal)
	{
		return x - proj(x, Normal);
	}
}//namespace glm
