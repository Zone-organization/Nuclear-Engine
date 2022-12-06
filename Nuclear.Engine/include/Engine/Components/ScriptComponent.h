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
			ScriptComponent(Assets::Script* _asset);
			~ScriptComponent();

			Assets::Script* GetScriptAsset(); 
			Scripting::ScriptingObject& GetScriptingObject();
		protected:
			Assets::Script* pParentScript;
			Scripting::ScriptingObject mScriptObject;
		};
	}
}