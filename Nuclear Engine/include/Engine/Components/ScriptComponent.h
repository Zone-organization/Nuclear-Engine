#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
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