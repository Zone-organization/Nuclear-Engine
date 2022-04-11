#pragma once
#include <Core\NE_Common.h>
#include <Core\Utilities\Hash.h>

namespace Nuclear
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
			std::string GetStringName() { return mName; }

			void SetName(const std::string& str) {
				mHashedName = Utilities::Hash(str);
				mName = str;
			}
			bool isValid = false;
		protected:
			Uint32 mHashedName = 0;
			std::string mName;
		};
	}
}