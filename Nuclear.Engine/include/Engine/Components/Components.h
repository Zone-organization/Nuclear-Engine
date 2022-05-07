#pragma once
#include <Engine/Components/EntityInfoComponent.h>
#include <Engine/Components/CameraComponent.h>
#include <Engine/Components/MeshComponent.h>
#include <Engine/Components/ColliderComponent.h>
#include <Engine/Components/RigidBodyComponent.h>
#include <Engine\Components\DirLightComponent.h>
#include <Engine\Components\PointLightComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Engine\Components\AnimatorComponent.h>
#include <Engine\Components\ScriptComponent.h>

#define ALL_COMPONENTS 	Components::EntityInfoComponent,\
					    Components::CameraComponent,    \
						Components::MeshComponent,      \
						Components::ColliderComponent,  \
						Components::RigidBodyComponent, \
						Components::DirLightComponent,  \
						Components::PointLightComponent,\
						Components::SpotLightComponent, \
						Components::AnimatorComponent,  \
						Components::ScriptComponent    