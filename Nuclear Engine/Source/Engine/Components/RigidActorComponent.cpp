#include<Engine\Components\RigidActorComponent.h>
namespace NuclearEngine
{
	namespace Components
	{
		RigidActorComponent::RigidActorComponent()
		{

		}

		RigidActorComponent::~RigidActorComponent()
		{
		}
		bool RigidActorComponent::GetType()
		{
			return Type;
		}
		PhysX::PxRigidActor* RigidActorComponent::GetActor()
		{
			if (Type == RigidActorStatic)
				return mStaticActor;
			else
				return mDynamicActor;
		}
	}
}