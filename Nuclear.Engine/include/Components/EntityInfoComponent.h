#pragma once
#include <ECS/Transform.h>
#include <ECS/Entity.h>

namespace Nuclear
{
	namespace Components
	{
		class NEAPI EntityInfoComponent
		{
		public:
			EntityInfoComponent(const ECS::Transform& Transform = ECS::Transform(), const std::string& name = "Entity");
			~EntityInfoComponent();

			ECS::Entity mOwnerEntity;
			ECS::Entity mParentEntity;
			ECS::Transform mTransform;
			std::string mName;
		};
	}
}