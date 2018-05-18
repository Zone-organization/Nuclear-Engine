/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#include "Engine/ECS/Event.h"


namespace NuclearEngine {

	namespace ECS {
		BaseEvent::Family BaseEvent::family_counter_ = 0;

		BaseEvent::~BaseEvent() {
		}

		EventManager::EventManager() {
		}

		EventManager::~EventManager() {
		}

	} 
}
