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

//# Platform Abstraction Layer Headers
#include <Platform\Clock.h>
#include <Platform\Timer.h>
#include <Platform\Input.h>

//# Managers Headers
#include <AssetManager\AssetManager.h>

//# Renderers Headers
#include <Renderers\Renderer3D.h>
#include <Renderers\SpriteRenderer.h>

//# Shading Headers
#include <Shading/Material.h>
#include <Shading/Technique.h>
#include <Shading/Techniques/NoLight.h>
#include <Shading/Techniques/PhongShading.h>

//# OneAPI Headers Begin
//---Buffers
#include <API/ConstantBuffer.h>
#include <API/VertexBuffer.h>
#include <API/IndexBuffer.h>
#include <API/InputLayout.h>
//---Textures
#include <API/Texture.h>
#include <API/Sampler.h>
#include <API/Texture_Types.h>
#include <API/RenderTarget.h>
//---RenderStates
#include <API/BlendState.h>
#include <API/RasterizerState.h>
#include <API/DepthStencilState.h>
#include <API/RenderStates_Types.h>
//---Shaders
#include <API/ShaderCompiler.h>
#include <API/Shader.h>
#include <API/VertexShader.h>
#include <API/PixelShader.h>
//---Other
#include <API/Color.h>
#include <API/Format.h>
#include <API/CommonStates.h>
//# OneAPI Headers End


//# Engine Components Headers
#include <Components\GenericCamera.h>
#include <Components\FlyCamera.h>
#include <Components\Skybox.h>
#include <Components\Light.h>
#include <Components\Model.h>
#include <Components\GUI\imgui.h>