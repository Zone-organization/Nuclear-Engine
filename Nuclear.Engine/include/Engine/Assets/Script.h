#pragma once
#include <Engine/Assets/Common.h>

struct _MonoMethod;
struct _MonoClass;
namespace Nuclear
{
	namespace Scripting
	{
		typedef _MonoMethod* ScriptFunction;
	}

	namespace Assets
	{

		class NEAPI Script : public Asset<Script>
		{
		public:
			Script();
			~Script();

			const std::string GetClassName();
			const std::string GetFullNameClassName();
			const std::string GetNamespaceName();

			Scripting::ScriptFunction mConstructor = nullptr;
			Scripting::ScriptFunction mLoadMethod = nullptr;
			Scripting::ScriptFunction mUpdateMethod = nullptr;
		//private:
			_MonoClass* Class = nullptr;


			std::string mFullName;
			std::string mClassName;
			std::string mNamespaceName;
		};
	}
}