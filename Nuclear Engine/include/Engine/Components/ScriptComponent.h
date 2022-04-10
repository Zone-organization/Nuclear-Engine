#pragma once
#include <Engine/Assets/Script.h>

namespace NuclearEngine
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