#pragma once
#include <NE_Common.h>
#include <Serialization/Access.h>
#include <string>

namespace Nuclear
{
	namespace Types
	{
		class TString
		{
		public:
			TString()
				: id(0), size(0), str("")
			{

			}

			TString(const std::string& _str)
			{
				size = _str.size();
				str = _str;
			}

			const std::string& string() const
			{
				return str;
			}
		protected:

			Uint32 id;
			size_t size;
			std::string str;

			//Serialization
			friend Serialization::Access;
			template<class S> void serialize(S& s)
			{
				s.value8b(size);

				//TODO: Probably find a better way to add all chars without iteriation
				for (size_t i = 0; i < str.size(); i++)
				{
					s.value1b(str[i]);
				}
			}
		};
	}
}