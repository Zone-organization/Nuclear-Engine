#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine/ECS/Transform.h>
#include <Engine/ECS/Entity.h>

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI EntityInfoComponent
		{
		public:
			EntityInfoComponent();
			EntityInfoComponent(Math::Matrix4 Transform);
			EntityInfoComponent(Math::Vector3 position, Math::Quaternion rotation);
			~EntityInfoComponent();

			ECS::Entity mOwnerEntity;
			ECS::Entity mParentEntity;
			ECS::Transform mTransform;
			std::string mName;
		};
	}
}