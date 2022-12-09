#include "Scripting/ScriptingAssembly.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Scripting
	{
		ScriptingAssembly::ScriptingAssembly()
		{
			pAssembly = nullptr;
			pImage = nullptr;
		}
		_MonoMethod* ScriptingAssembly::GetMethod(const std::string& methodname)
		{
			MonoMethodDesc* description = mono_method_desc_new(methodname.c_str(), NULL);;
			return mono_method_desc_search_in_image(description, pImage);
		}
		const std::string& ScriptingAssembly::GetNamespaceName() const
		{
			return mNamespaceName;
		}
		_MonoAssembly* ScriptingAssembly::GetAssembly()
		{
			return pAssembly;
		}
		_MonoImage* ScriptingAssembly::GetImage()
		{
			return pImage;
		}
	}
}