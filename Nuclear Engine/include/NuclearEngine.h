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
#include <Core/Systems/AudioSystem.h>

//# Managers Headers
#include <ResourceManager\ResourceManager.h>

#include <Shading/Material.h>

//# OneAPI Headers
#include <API/VertexBuffer.h>
#include <API/IndexBuffer.h>
#include <API/UniformBuffer.h>
#include <API/InputLayout.h>
#include <API/Texture.h>
#include <API/DepthStencilState.h>
#include <API/Shader.h>
#include <API/Color.h>

//# Engine Components Headers
#include <Components\GenericCamera.h>
#include <Components\FlyCamera.h>
#include <Components\Skybox.h>
#include <Components\Light.h>
#include <Components\Model.h>
#include <Components\Models\Cube.h>

