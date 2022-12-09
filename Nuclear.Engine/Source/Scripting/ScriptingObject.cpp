#include <Scripting/ScriptingObject.h>
#include <Scripting/ScriptingClass.h>
#include <Utilities/Logger.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>

namespace Nuclear
{
	namespace Scripting
	{
		ScriptFunction ScriptingObject::GetVirtualMethod(const std::string& methodname)
		{
			MonoMethodDesc* methoddesc = mono_method_desc_new(methodname.c_str(), false);

			MonoMethod* result = nullptr;
			if (methoddesc)
			{
				// Get real function
				MonoMethod* virtualMethod = mono_method_desc_search_in_class(methoddesc, pParent->GetClassPtr());
				if (virtualMethod)
				{
					result = mono_object_get_virtual_method(GetMonoObjectPtr(), virtualMethod);
				}

				// Free
				mono_method_desc_free(methoddesc);
			}

			return result;
		}
		_MonoObject* ScriptingObject::CallMethod(Scripting::ScriptFunction method, void** params)
		{
			if (method != nullptr)
			{
				MonoObject* pException = nullptr;
				MonoObject* result = mono_runtime_invoke(method, GetMonoObjectPtr(), params, &pException);

				// Report exception
				if (pException) {
					MonoString* exString = mono_object_to_string(pException, nullptr);
					const char* exCString = mono_string_to_utf8(exString);
					NUCLEAR_ERROR("[ScriptingObject] Failed to call C# Method, Mono Error: {0}" , exCString);
				}
				return result;
			}
			return nullptr;
		}
		ScriptingClass* ScriptingObject::GetScriptingClass()
		{
			return pParent;
		}
		_MonoObject* ScriptingObject::GetMonoObjectPtr() const
		{
			return mono_gchandle_get_target(mHandle);
		}
	}
}