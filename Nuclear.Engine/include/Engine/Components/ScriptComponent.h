#pragma once
#include <Engine/Assets/Script.h>

namespace Nuclear
{
	namespace Components
	{

		class NEAPI ScriptComponent
		{
		public:
			ScriptComponent();
			
			Assets::Script* script;
		};
	}
}