#pragma once
#include <Core/NE_Common.h>
#include <Engine/Assets/Script.h>

struct _MonoObject;

namespace Nuclear
{
	namespace Scripting
	{

		class NEAPI ScriptingObject
		{
		public:
			_MonoObject* GetMonoObject() const;

			_MonoObject* CallMethod(Scripting::ScriptFunction method, void** params = nullptr);

			Assets::Script* GetScript();
		private:
			friend class ScriptingEngine;
			Uint32 mHandle = 0;
			Assets::Script* pParent;
		};
	}
}
