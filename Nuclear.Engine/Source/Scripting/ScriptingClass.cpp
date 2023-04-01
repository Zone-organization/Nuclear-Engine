#include <Scripting/ScriptingClass.h>
#include <Scripting/ScriptingModule.h>
#include <Utilities/Logger.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
namespace Nuclear
{
	namespace Scripting
	{
		ScriptingObject ScriptingClass::CreateObject()
		{
			ScriptingObject result;

			MonoObject* instance = mono_object_new(ScriptingModule::GetInstance().GetDomain(), pClass);

			if (!instance)
			{
				NUCLEAR_ERROR("[ScriptingClass : {0}] Failed Create object C# class!", mDesc.mClassName);
				return result;
			}

			mono_runtime_object_init(instance);
			result.mHandle = mono_gchandle_new(instance, false);
			return result;
		}
		ScriptFunction ScriptingClass::GetMethod(const std::string& methodname)
		{
			MonoMethodDesc* methodDesc = mono_method_desc_new((mDesc.mClassName + "::" + methodname).c_str(), false);
			return mono_method_desc_search_in_class(methodDesc, pClass);
		}
		_MonoClass* ScriptingClass::GetClassPtr()
		{
			return pClass;
		}

	}
}