#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include "Engine\ECS/System.h"
#include <Engine/Graphics/API/RenderAPI.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\Sprite.h>
#include <vector>

namespace NuclearEngine
{
	namespace Systems
	{
		struct NEAPI SpriteRenderSystemDesc
		{
			Uint32 Width = 800;
			Uint32 Height = 600;

			Graphics::API::ShaderLoadDesc VertexShaderPath = Graphics::API::ShaderLoadDesc("Assets/NuclearEngine/Shaders/SpriteRenderSystem.vs.hlsl");
			Graphics::API::ShaderLoadDesc PixelShaderPath = Graphics::API::ShaderLoadDesc("Assets/NuclearEngine/Shaders/SpriteRenderSystem.ps.hlsl");
			Components::CameraComponent* InitialCamera = nullptr;
		};
		//Renderer for 2D Components
		class NEAPI SpriteRenderSystem : public ECS::System<SpriteRenderSystem> {
		public:
			SpriteRenderSystem();
	
			bool Initialize(const SpriteRenderSystemDesc& desc);

			bool SetActiveCamera(Components::CameraComponent* Camera);

			void BeginDirectRender();
			void DirectRender(Components::Sprite* sprite);
			void EndDirectRender();
			//Update Functions
			void Update(ECS::EntityManager &es, ECS::EventManager &events, ECS::TimeDelta dt) override;

		private:
			Graphics::API::VertexShader mVShader;
			Graphics::API::PixelShader mPShader;
			Graphics::API::VertexBuffer mVertexBuffer;
			Graphics::API::ConstantBuffer mSpriteColorBuffer;
			Components::CameraComponent* mCamera;
		};

	}
}
