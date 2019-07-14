#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <Engine/ECS/Transform.h>

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI TransformComponent : public ECS::Component<TransformComponent>
		{
		public:
			TransformComponent();
			TransformComponent(Math::Matrix4 Transform);
			TransformComponent(Math::Vector3 position, Math::Quaternion rotation);
			~TransformComponent();

			ECS::Transform mTransform;
		};
	}
}