#pragma once
#include <Engine/Assets/Common.h>
#include <Engine/Scripting/ScriptingClass.h>

namespace Nuclear
{
	namespace Assets
	{

		class NEAPI Script : public Asset<Script>
		{
		public:
			Script();
			~Script();


			Scripting::ScriptFunction mConstructor = nullptr;
			Scripting::ScriptFunction mOnStartMethod = nullptr;
			Scripting::ScriptFunction mOnUpdateMethod = nullptr;

			Scripting::ScriptingClass mClass;

		};
	}
}