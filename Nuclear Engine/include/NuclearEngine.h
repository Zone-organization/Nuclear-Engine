#pragma once
/*
          _ ._  _ , _ ._
        (_ ' ( `  )_  .__)
      ( (  (    )   `)  ) _)
     (__ (_   (_ . _) _) ,__)
         `~~`\ ' . /`~~`
              ;   ;
              /   \
_____________/BOOOM\_____________
*/

//# Core Engine Headers
#include <Core/Engine.h>
#include <Core/Game.h>
#include <Core/Editor.h>
#include <Core/Input.h>
#include <Core/Application.h>
#include <Core/FileSystem.h>
#include <Core/Parsers/XMLParser.h>

//--Entity Component System Headers
#include <Engine/ECS/Scene.h>
#include <Engine/ECS/Event.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/System.h>

#include <Engine/Components/TransformComponent.h>

#include <Base\Utilities\Hash.h>

//# Assets & their managers Headers
#include <Engine\Managers\AssetManager.h>
#include <Engine\Managers\ShaderManager.h>

//# Asset Classes
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>

//# Audio SubSystem Headers
#include <Engine/Audio/AudioEngine.h>
#include <Engine/Audio/Sound.h>
#include <Engine/Audio/Channel.h>

//# Physics3D Headers
#include <Engine/Physics3D/Physics3DEngine.h>
#include <Engine/Physics3D/PhysicsScene.h>

//# Graphics Headers
#include <Engine\Graphics\Skybox.h>
#include <Engine\Graphics\Background.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <Engine\Graphics\RenderingPipelines\DiffuseOnly.h>
#include <Engine\Graphics\RenderingPipelines\BlinnPhong.h>

//# Engine Components Headers
#include <Engine/Components\CameraComponent.h>
#include <Engine/Components\Light.h>
#include <Engine/Components\MeshComponent.h>

//# Engine Systems Headers
#include <Engine/Systems\RenderSystem.h>
