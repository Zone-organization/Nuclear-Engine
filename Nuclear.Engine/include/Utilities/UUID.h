#pragma once
#include <NE_Common.h>
#include <array>
#include <sstream>
#include <string_view>

namespace Nuclear
{
	namespace Utilities
	{

		// Class to represent a GUID/UUID. Each instance acts as a wrapper around a
		// 16 byte value that can be passed around by value. It also supports
		// conversion to string (via the stream operator <<) and conversion from a
		// string via constructor.
		class NEAPI UUID
		{
		public:
			explicit UUID(const std::array<unsigned char, 16>& bytes);
			explicit UUID(std::array<unsigned char, 16>&& bytes);

			explicit UUID(std::string_view fromString);
			UUID();

			UUID(const UUID& other) = default;
			UUID& operator=(const UUID& other) = default;
			UUID(UUID&& other) = default;
			UUID& operator=(UUID&& other) = default;

			bool operator==(const UUID& other) const;
			bool operator!=(const UUID& other) const;

			std::string str() const;
			operator std::string() const;
			const std::array<unsigned char, 16>& bytes() const;
			void swap(UUID& other);
			bool isValid() const;

			static UUID CreateNewUUID();

		private:
			void zeroify();

			// actual data
			std::array<unsigned char, 16> _bytes;

			// make the << operator a friend so it can access _bytes
			friend std::ostream& operator<<(std::ostream& s, const UUID& guid);
			friend bool operator<(const UUID& lhs, const UUID& rhs);
		};

		namespace Internal
		{
			template <typename...> struct hash;

			template<typename T>
			struct hash<T> : public std::hash<T>
			{
				using std::hash<T>::hash;
			};


			template <typename T, typename... Rest>
			struct hash<T, Rest...>
			{
				inline std::size_t operator()(const T& v, const Rest&... rest) {
					std::size_t seed = hash<Rest...>{}(rest...);
					seed ^= hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
					return seed;
				}
			};
		}
	}
}

namespace std
{
	// Template specialization for std::swap<Guid>() --
	// See guid.cpp for the function definition
	template <>
	void swap(Nuclear::Utilities::UUID& guid0, Nuclear::Utilities::UUID& guid1) noexcept;

	// Specialization for std::hash<Guid> -- this implementation
	// uses std::hash<std::string> on the stringification of the guid
	// to calculate the hash
	template <>
	struct hash<Nuclear::Utilities::UUID>
	{
		std::size_t operator()(Nuclear::Utilities::UUID const& guid) const
		{
			const uint64_t* p = reinterpret_cast<const uint64_t*>(guid.bytes().data());
			return Nuclear::Utilities::Internal::hash<uint64_t, uint64_t>{}(p[0], p[1]);
		}
	};
}


//Modified Code from https://github.com/graeme-hill/crossguid
/*
The MIT License (MIT)

Copyright (c) 2014 Graeme Hill (http://graemehill.ca)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/