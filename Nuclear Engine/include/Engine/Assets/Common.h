#pragma once
#include <Base\NE_Common.h>
#include <Base\Utilities\Hash.h>

namespace NuclearEngine
{
	namespace Assets
	{

		template <class T>
		class Asset
		{
		public:
			const char* GetType()
			{
				return typeid(T).name();
			}

			Uint32 GetName() { return 	mHashedName; }
			void SetName(Uint32 name) { mHashedName = name; }
			void SetName(const std::string& str) { mHashedName = Utilities::Hash(str); }
			bool isValid = false;
		protected:
			Uint32 mHashedName = 0;
		};
	}
}