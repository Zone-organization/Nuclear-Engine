#pragma once
#include <Core\NE_Common.h>
#include <Engine/ECS/Common.h>
#include <Engine/ECS/entt/entt.hpp>

namespace NuclearEngine
{
	namespace ECS
	{
		class NEAPI Entity 
		{
		public:
			template<typename Component, typename... Args>
			decltype(auto) AddComponent(Args &&...args)
			{
				return parent->emplace<Component>(entity, std::forward<Args>(args)...);
			}

			template<typename... Component>
			auto GetComponent() const 
			{
				if constexpr (sizeof...(Component) == 1) {
					return (const_cast<Component*>(std::as_const(*this).template parent->try_get<Component>(entity)), ...);
				}
				else {
					return std::make_tuple(parent->try_get<Component>(entity)...);
				}
			}

			entt::registry* parent;
			entt::entity entity;
		};
	}
}