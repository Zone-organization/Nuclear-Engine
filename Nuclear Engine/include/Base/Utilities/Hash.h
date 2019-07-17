#pragma once
#include <ThirdParty\crc32c.h>
#include <Base\NE_PrivateCommon.h>
#include <string>
#include <sstream>

namespace NuclearEngine
{
	namespace Utilities
	{
		inline Uint32 NEAPI Hash(const char* data, size_t count)
		{
			return crc32c::Crc32c(data, count);
		}

		inline Uint32 NEAPI Hash(const std::string& string)
		{
			return crc32c::Crc32c(string);
		}

		template< typename T >
		std::string int_to_hex(T i)
		{
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(sizeof(T) * 2)
				<< std::hex << i;
			return stream.str();
		}
	}
}