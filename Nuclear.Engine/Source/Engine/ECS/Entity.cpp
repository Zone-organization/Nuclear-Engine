#include "Engine/ECS/Entity.h"

namespace Nuclear
{
	namespace ECS
	{
		Entity::Entity()
			:pRegistry(nullptr), mEntity(entt::null)
		{
		}
		Entity::Entity(entt::registry& registry, Uint32 enttid)
			:pRegistry(&registry), mEntity(static_cast<entt::entity>(enttid))
		{
		}
		Entity::Entity(entt::registry& registry ,const entt::entity& entityid)
			:pRegistry(&registry), mEntity(entityid)
		{
		}
		Entity::~Entity()
		{
			pRegistry = nullptr;
			mEntity = entt::null;
		}
		entt::registry* Entity::GetRegistry()
		{
			return pRegistry;
		}
		entt::entity& Entity::GetEntityID()
		{
			return mEntity;
		}
	}
}