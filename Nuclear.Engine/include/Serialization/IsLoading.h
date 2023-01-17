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
