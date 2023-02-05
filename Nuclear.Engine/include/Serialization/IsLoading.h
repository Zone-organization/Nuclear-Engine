#pragma once
#include <ThirdParty/zpp_bits.h>

#define ISLOADING(archive) \
using archive_type = std::remove_cvref_t<decltype(archive)>; \
if constexpr (archive_type::kind() == zpp::bits::kind::in) \

