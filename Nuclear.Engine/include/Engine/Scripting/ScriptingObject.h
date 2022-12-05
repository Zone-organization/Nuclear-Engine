#pragma once
#include <Core/NE_Common.h>
#include <Engine/Scripting/MonoDeclarations.h>
#include <string>
namespace Nuclear
{
	namespace Scripting
	{
		class ScriptingClass;
		class NEAPI ScriptingObject
		{
		public:
			_MonoObject* GetMonoObjectPtr() const;

			ScriptFunction GetVirtualMethod(const std::string& methodname);
			_MonoObject* CallMethod(ScriptFunction method, void** params = nullptr);

			ScriptingClass* GetScriptingClass();
		private:
			friend class ScriptingClass;
			Uint32 mHandle = 0;
			ScriptingClass* pParent;
		};
	}
}
