#include <Engine/Scripting/ScriptingObject.h>
#include <Core\Logger.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Nuclear
{
	namespace Scripting
	{
		_MonoObject* ScriptingObject::CallMethod(Scripting::ScriptFunction method, void** params)
		{
			if (method != nullptr)
			{
				MonoObject* pException = nullptr;
				MonoObject* result = mono_runtime_invoke(method, GetMonoObject(), params, &pException);
				if(!pException)
				{
					NUCLEAR_ERROR("[ScriptingSystem] Failed to call C# Method!");
				}
				return result;
			}
			return nullptr;
		}
		Assets::Script* ScriptingObject::GetScript()
		{
			return pParent;
		}
		_MonoObject* ScriptingObject::GetMonoObject() const
		{
			return mono_gchandle_get_target(mHandle);
		}
	}
}