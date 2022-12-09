#pragma once
#include <Components/EntityInfoComponent.h>
#include <Components/CameraComponent.h>
#include <Components/MeshComponent.h>
#include <Components/ColliderComponent.h>
#include <Components/RigidBodyComponent.h>
#include <Components\DirLightComponent.h>
#include <Components\PointLightComponent.h>
#include <Components\SpotLightComponent.h>
#include <Components\ScriptComponent.h>

#define ALL_COMPONENTS 	Components::EntityInfoComponent,\
					    Components::CameraComponent,    \
						Components::MeshComponent,      \
						Components::ColliderComponent,  \
						Components::RigidBodyComponent, \
						Components::DirLightComponent,  \
						Components::PointLightComponent,\
						Components::SpotLightComponent, \
						Components::ScriptComponent    