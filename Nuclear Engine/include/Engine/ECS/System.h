/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#pragma once


#include <cstdint>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <Base\NE_Common.h>
#include "Engine\ECS/ECSConfig.h"
#include "Engine\ECS/Entity.h"
#include "Engine\ECS/Event.h"
#include "Base\Utilities/NonCopyable.h"

namespace NuclearEngine {
	namespace ECS {

		class SystemManager;


		/**
		 * Base System class. Generally should not be directly used, instead see System<Derived>.
		 */
		class NEAPI BaseSystem : Utilities::NonCopyable {
		public:
			typedef size_t Family;

			virtual ~BaseSystem();

			/**
			 * Called once all Systems have been Added to the SystemManager.
			 *
			 * Typically used to set up event handlers.
			 */
			virtual void Configure(EntityManager &entities, EventManager &events) {
				Configure(events);
			}

			/**
			 * Legacy Configure(). Called by default implementation of Configure(EntityManager&, EventManager&).
			 */
			virtual void Configure(EventManager &events) {}

			/**
			 * Apply System behavior.
			 *
			 * Called every game step.
			 */
			virtual void Update(EntityManager &entities, EventManager &events, TimeDelta dt) = 0;

			static Family family_counter_;

		protected:
		};


		/**
		 * Use this class when implementing Systems.
		 *
		 * struct MovementSystem : public System<MovementSystem> {
		 *   void Update(EntityManager &entities, EventManager &events, TimeDelta dt) {
		 *     // Do stuff to/with entities...
		 *   }
		 * }
		 */
		template <typename Derived>
		class System : public BaseSystem {
		public:
			virtual ~System() {}

		private:
			friend class SystemManager;

			static Family family() {
				static Family family = family_counter_++;
				return family;
			}
		};


		class NEAPI SystemManager : Utilities::NonCopyable {
		public:
			SystemManager(EntityManager &entity_manager,
				EventManager &event_manager) :
				entity_manager_(entity_manager),
				event_manager_(event_manager) {}

			/**
			 * Add a System to the SystemManager.
			 *
			 * Must be called before Systems can be used.
			 *
			 * eg.
			 * std::shared_ptr<MovementSystem> movement = entityx::make_shared<MovementSystem>();
			 * system.Add(movement);
			 */
			template <typename S>
			void Add(std::shared_ptr<S> system) {
				systems_.insert(std::make_pair(S::family(), system));
			}

			/**
			 * Add a System to the SystemManager.
			 *
			 * Must be called before Systems can be used.
			 *
			 * eg.
			 * auto movement = system.Add<MovementSystem>();
			 */
			template <typename S, typename ... Args>
			std::shared_ptr<S> Add(Args && ... args) {
				std::shared_ptr<S> s(new S(std::forward<Args>(args) ...));
				Add(s);
				return s;
			}

			/**
			 * Retrieve the registered System instance, if any.
			 *
			 *   std::shared_ptr<CollisionSystem> collisions = systems.system<CollisionSystem>();
			 *
			 * @return System instance or empty shared_std::shared_ptr<S>.
			 */
			template <typename S>
			std::shared_ptr<S> GetSystem() {
				auto it = systems_.find(S::family());
				assert(it != systems_.end());
				return it == systems_.end()
					? std::shared_ptr<S>()
					: std::shared_ptr<S>(std::static_pointer_cast<S>(it->second));
			}

			/**
			 * Call the System::Update() method for a registered system.
			 */
			template <typename S>
			void Update(TimeDelta dt) {
				assert(initialized_ && "SystemManager::Configure() not called");
				std::shared_ptr<S> s = system<S>();
				s->Update(entity_manager_, event_manager_, dt);
			}

			/**
			 * Call System::Update() on all registered systems.
			 *
			 * The order which the registered systems are Updated is arbitrary but consistent,
			 * meaning the order which they will be Updated cannot be specified, but that order
			 * will stay the same as long no systems are Added or Removed.
			 *
			 * If the order in which systems Update is important, use SystemManager::Update()
			 * to manually specify the Update order. EntityX does not yet support a way of
			 * specifying priority for Update_All().
			 */
			void Update_All(TimeDelta dt);

			/**
			 * Configure the system. Call after Adding all Systems.
			 *
			 * This is typically used to set up event handlers.
			 */
			void Configure();

		private:
			bool initialized_ = false;
			EntityManager &entity_manager_;
			EventManager &event_manager_;
			std::unordered_map<BaseSystem::Family, std::shared_ptr<BaseSystem>> systems_;
		};
	}
}