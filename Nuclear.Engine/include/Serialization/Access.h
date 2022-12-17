#pragma once
#include <bitsery/details/serialization_common.h>
#include <bitsery/ext/inheritance.h>
#include <bitsery/ext/std_map.h>

namespace Nuclear
{
	namespace Serialization
	{
		using Access = bitsery::Access;
		using StdMap = bitsery::ext::StdMap;

		using bitsery::ext::BaseClass;
		using bitsery::ext::VirtualBaseClass;
	}
}