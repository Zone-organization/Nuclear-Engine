#pragma once
#include <Core\NE_Common.h>
#include <Engine/ECS/Common.h>
#include <Engine/ECS/entt/entt.hpp>

namespace Nuclear
{
	namespace ECS
	{
		class NEAPI Entity 
		{
		public:
			Entity();
			Entity(entt::registry& registry, Uint32 enttid);
			Entity(entt::registry& registry,const entt::entity& entityid);
			~Entity();

			template<typename Component, typename... Args>
			decltype(auto) AddComponent(Args &&...args)
			{
				return pRegistry->emplace<Component>(mEntity, std::forward<Args>(args)...);
			}

			template<typename... Component>
			[[nodiscard]] decltype(auto) GetComponent() const
			{
				return pRegistry->get<Component...>(mEntity);
			}

			template<typename... Component>
			auto TryToGetComponent() const 
			{
				if constexpr (sizeof...(Component) == 1) {
					return (const_cast<Component*>(std::as_const(*this).template pRegistry->try_get<Component>(mEntity)), ...);
				}
				else {
					return std::make_tuple(pRegistry->try_get<Component>(mEntity)...);
				}
			}


			template<typename... Component>
			bool HasComponent() const
			{
				return pRegistry->all_of<Component...>(mEntity);
			}

			entt::registry* GetRegistry();
			entt::entity& GetEntityID();

		private:
			entt::registry* pRegistry;
			entt::entity mEntity;
		};
	}
}