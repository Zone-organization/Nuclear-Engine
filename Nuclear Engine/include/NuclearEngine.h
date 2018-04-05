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
//Note: The Supported Renderers will expand when I get new hardware, currently planned in the future ( DirectX 12 , Vulkan )

//# Core Engine Headers
#include <Core/Engine.h>
#include <Core/Game.h>
#include <Core/Editor.h>
#include <Core/FileSystem.h>
//--Entity Component System Headers
#include <Core/Scene.h>
#include <Core/Event.h>
#include <Core/Entity.h>
#include <Core/System.h>

//# Platform Abstraction Layer Headers
#include <Platform\Clock.h>
#include <Platform\Timer.h>
#include <Platform\Input.h>

//# XAsset & their managers Headers
#include <Managers\AssetManager.h>
#include <Managers\ShaderManager.h>

//# Asset Classes (XAsset)
#include <XAsset\ModelAsset.h>

//# Audio SubSystem Headers
#include <Audio/AudioEngine.h>
#include <Audio/Sound.h>
#include <Audio/Channel.h>

//# Physics3D Headers
#include <Physics3D/Physics3DEngine.h>
#include <Physics3D/PhysicsScene.h>

//# OneAPI Headers
#include <Graphics/API/RenderAPI.h>

//# GUI Headers
#include <GUI\IMGUI.h>

//# Engine Components Headers
#include <Components\GenericCamera.h>
#include <Components\FlyCamera.h>
#include <Components\Skybox.h>
#include <Components\Light.h>
#include <Components\Model.h>
#include <Components\Sprite.h>
#include <Components\Transform.h>
#include <Components\ModelRenderDesc.h>

//# Engine Systems Headers
#include <Systems\RenderSystem.h>
#include <Systems\SpriteRenderSystem.h>