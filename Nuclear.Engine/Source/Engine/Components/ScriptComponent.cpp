#include <Engine\Components\ScriptComponent.h>

namespace Nuclear
{
	namespace Components
	{
		ScriptComponent::ScriptComponent()
			: pParentScript(nullptr)
		{
		}
		ScriptComponent::ScriptComponent(Assets::Script* _asset)
			: pParentScript(_asset)
		{
		}
		ScriptComponent::~ScriptComponent()
		{
		}
		Assets::Script* ScriptComponent::GetScriptAsset()
		{
			return pParentScript;
		}
		Scripting::ScriptingObject& ScriptComponent::GetScriptingObject()
		{
			return mScriptObject;
		}
	}
}