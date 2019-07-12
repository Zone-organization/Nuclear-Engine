#include "Engine\Components\ColliderComponent.h"

namespace NuclearEngine
{
	namespace Components
	{
		ColliderComponent::ColliderComponent()
		{
		}
		ColliderComponent::ColliderComponent(PhysX::PhysXMaterial Pmat)
		{
			mMaterial = Pmat;
		}
		ColliderComponent::~ColliderComponent()
		{
		}
	}
}