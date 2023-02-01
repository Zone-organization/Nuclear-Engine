#pragma once

constexpr bool IsLoading(auto& archive)
{
	using archive_type = std::remove_cvref_t<decltype(archive)>;

	if constexpr (archive_type::kind() == zpp::bits::kind::in)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Sometimes compilers fails to evaluate the original function as compile-time constexpr
//use this instead
#define ISLOADING(archive) \
using archive_type = std::remove_cvref_t<decltype(archive)>; \
if constexpr (archive_type::kind() == zpp::bits::kind::in) \

