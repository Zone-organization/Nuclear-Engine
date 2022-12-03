#include "Engine/Scripting/ScriptingAssembly.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <Core\Logger.h>

namespace Nuclear
{
	namespace Scripting
	{
		_MonoMethod* ScriptingAssembly::GetMethod(const std::string& methodname)
		{

			MonoMethodDesc* description = mono_method_desc_new(methodname.c_str(), NULL);
			if (!description)
			{
				NUCLEAR_ERROR("[ScriptingAssembly] Failed to create {0} method desc!" , methodname);
			}
			MonoMethod* method = mono_method_desc_search_in_image(description, pImage);
			if (!method)
			{
				NUCLEAR_ERROR("[ScriptingAssembly] Method {0} does not exist in the image!", methodname);
			}
			return method;
		}
	}
}