#pragma once
/*
* Copyright (C) 2013 Alec Thomas <alec@swapoff.org>
* All rights reserved.
*
* This software is licensed as described in the file COPYING, which
* you should have received as part of this distribution.
*
* Author: Alec Thomas <alec@swapoff.org>
*/
#pragma once

#include "System.h"
#include "Event.h"
#include "Entity.h"

namespace NuclearEngine {
	namespace ECS {

		/**
		* An entityx::System for declaring component dependencies.
		*
		* eg. To declare that a `Physics` component must always be paired with `Position`
		* and `Direction` components:
		*
		*     system_manager->add<Dependency<Physics, Position, Direction>>();
		*/
		template <typename C, typename ... Deps>
		class Dependency : public System<Dependency<C, Deps...>>, public Receiver<Dependency<C, Deps...>> {
		public:
			void Receive(const ComponentAddedEvent<C> &event) {
				this->Assign<Deps...>(event.entity);
			}

			void Configure(EventManager &events) override {
				events.Subscribe<ComponentAddedEvent<C>>(*this);
			}

			void Update(EntityManager &entities, EventManager &events, TimeDelta dt) override {}

		private:
			template <typename D>
			void Assign(Entity entity)  {
				if (!entity.GetComponent<D>())
					entity.Assign<D>();
			}

			template <typename D, typename D1, typename ... Ds>
			void Assign(Entity entity) {
				Assign<D>(entity);
				Assign<D1, Ds...>(entity);
			}
		};
	}
}