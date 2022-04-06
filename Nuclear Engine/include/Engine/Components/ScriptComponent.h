#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine/Assets/Script.h>

namespace NuclearEngine
{
	namespace Components
	{

		class NEAPI ScriptComponent : public ECS::Component<ScriptComponent>
		{
		public:
			ScriptComponent();
			
			Assets::Script* script;
		};
	}
}