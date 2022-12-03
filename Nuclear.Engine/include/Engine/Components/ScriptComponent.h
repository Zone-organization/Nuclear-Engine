#pragma once
#include <Engine/Assets/Script.h>
#include <Engine/Scripting/ScriptingObject.h>

namespace Nuclear
{
	namespace Components
	{

		class NEAPI ScriptComponent
		{
		public:
			ScriptComponent();
			
			Scripting::ScriptingObject mScriptObject;
		};
	}
}