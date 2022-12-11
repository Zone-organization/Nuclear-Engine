#include "Components/EntityInfoComponent.h"

namespace Nuclear
{
	namespace Components
	{
		EntityInfoComponent::EntityInfoComponent(const ECS::Transform &Transform, const std::string& name)
			: mName(name), mTransform(Transform)
		{
		}

		EntityInfoComponent::~EntityInfoComponent()
		{
		}
	}
}