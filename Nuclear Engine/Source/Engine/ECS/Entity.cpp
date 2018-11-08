/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#include <algorithm>
#include "Engine/ECS/Entity.h"


namespace NuclearEngine {

	namespace ECS {

		const Entity::Id Entity::INVALID;
		BaseComponent::Family BaseComponent::family_counter_ = 0;

		void Entity::InValidate() {
			id_ = INVALID;
			manager_ = nullptr;
		}

		void Entity::Destroy() {
			assert(Valid());
			manager_->Destroy(id_);
			InValidate();
		}

		std::bitset<ECS::MAX_COMPONENTS> Entity::component_mask() const {
			return manager_->component_mask(id_);
		}

		EntityManager::EntityManager(EventManager &event_manager) : event_manager_(event_manager) {
		}

		EntityManager::~EntityManager() {
			reset();
		}

		void EntityManager::reset() {
			for (Entity entity : entities_for_debugging()) entity.Destroy();
			for (Utilities::BasePool *pool : component_pools_) {
				if (pool) delete pool;
			}
			for (BaseComponentHelper *helper : component_helpers_) {
				if (helper) delete helper;
			}
			component_pools_.clear();
			component_helpers_.clear();
			entity_component_mask_.clear();
			entity_version_.clear();
			free_list_.clear();
			index_counter_ = 0;
		}
	} 
}