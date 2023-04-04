#pragma once
#include <Components/AudioSourceComponent.h>
#include <Components/AudioListenerComponent.h>
#include <Components/CameraComponent.h>
#include <Components/EntityInfoComponent.h>
#include <Components/MeshComponent.h>
#include <Components/ColliderComponent.h>
#include <Components/RigidBodyComponent.h>
#include <Components\LightComponent.h>
#include <Components\ScriptComponent.h>

#define ALL_COMPONENTS 	Components::EntityInfoComponent,\
					    Components::CameraComponent,    \
						Components::MeshComponent,      \
						Components::ColliderComponent,  \
						Components::RigidBodyComponent, \
						Components::LightComponent,  \
						Components::ScriptComponent,    \
						Components::AudioSourceComponent,  \
						Components::AudioListenerComponent