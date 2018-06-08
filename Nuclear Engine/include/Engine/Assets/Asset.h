#pragma once
#include <Base\Utilities\Hash.h>

namespace NuclearEngine
{
	namespace Assets 
	{
		class Asset
		{
		public:
			Uint32 GetName() {	return 	mHashedName; }
			void SetName(Uint32 name) { mHashedName = name; }
			void SetName(const std::string& str) { mHashedName = Utilities::Hash(str); }
			bool isValid = false;
		protected:
			Uint32 mHashedName;			
		};
	}
}