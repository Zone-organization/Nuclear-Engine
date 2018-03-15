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
#include <Core/Context.h>
#include <Core/Game.h>
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

//# Shading Headers
#include <Shading/Material.h>


//# OneAPI Headers
#include <API/RenderAPI.h>


//# Engine Components Headers
#include <Components\GenericCamera.h>
#include <Components\FlyCamera.h>
#include <Components\Skybox.h>
#include <Components\Light.h>
#include <Components\Model.h>
#include <Components\Sprite.h>
#include <Components\GUI\imgui.h>

//# Engine Systems Headers
#include <Systems\RenderSystem.h>
#include <Systems\SpriteRenderSystem.h>