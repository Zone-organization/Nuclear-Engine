#pragma once
#include <NE_Common.h>
#include <Core/Entity.h>
#include "Core/Event.h"
#include "Core/System.h"
#include <Graphics/API/RenderAPI.h>
#include <Components\GenericCamera.h>
#include <Components\Sprite.h>
#include <vector>

namespace NuclearEngine
{
	namespace Systems
	{
		struct SpriteRenderSystemDesc
		{
			Uint32 Width = 800;
			Uint32 Height = 600;

			Graphics::API::ShaderLoadDesc VertexShaderPath = Graphics::API::ShaderLoadDesc("Assets/NuclearEngine/Shaders/SpriteRenderSystem.vs.hlsl");
			Graphics::API::ShaderLoadDesc PixelShaderPath = Graphics::API::ShaderLoadDesc("Assets/NuclearEngine/Shaders/SpriteRenderSystem.ps.hlsl");
			Components::GenericCamera* InitialCamera = nullptr;
		};
		//Renderer for 2D Components
		class SpriteRenderSystem : public Core::System<SpriteRenderSystem> {
		public:
			SpriteRenderSystem();
	
			bool Initialize(const SpriteRenderSystemDesc& desc);

			bool SetActiveCamera(Components::GenericCamera* Camera);

			void BeginDirectRender();
			void DirectRender(Components::Sprite* sprite);
			void EndDirectRender();
			//Update Functions
			void Update(Core::EntityManager &es, Core::EventManager &events, Core::TimeDelta dt) override;

		private:
			Graphics::API::VertexShader mVShader;
			Graphics::API::PixelShader mPShader;
			Graphics::API::VertexBuffer mVertexBuffer;
			Graphics::API::ConstantBuffer mSpriteColorBuffer;
			Components::GenericCamera* mCamera;
		};

	}
}

//Implementation (Required since EntityX and C++ Templates doesn't play nicely with DLL Export)
#define SpriteRenderSystemImpl
#include "SpriteRenderSystemImpl.inl"
#undef SpriteRenderSystemImpl