#include "Engine\Components/EntityInfoComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <Core/Math/gtx/quaternion.hpp>
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