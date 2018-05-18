/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#include "Engine/ECS/System.h"


namespace NuclearEngine {

	namespace ECS {
		BaseSystem::Family BaseSystem::family_counter_;

		BaseSystem::~BaseSystem() {
		}

		void SystemManager::Update_All(TimeDelta dt) {
			assert(initialized_ && "SystemManager::configure() not called");
			for (auto &pair : systems_) {
				pair.second->Update(entity_manager_, event_manager_, dt);
			}
		}

		void SystemManager::Configure() {
			for (auto &pair : systems_) {
				pair.second->Configure(entity_manager_, event_manager_);
			}
			initialized_ = true;
		}

	}
}