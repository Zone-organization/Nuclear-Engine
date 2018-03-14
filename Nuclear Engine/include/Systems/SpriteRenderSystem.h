#pragma once
#include <NE_Common.h>
#include <Core/Entity.h>
#include "Core/Event.h"
#include "Core/System.h"
#include <API\RenderAPI.h>
#include <Components\GenericCamera.h>
#include <vector>

namespace NuclearEngine
{
	namespace Systems
	{
		struct SpriteRenderSystemDesc
		{
			Uint32 Width = 800;
			Uint32 Height = 600;

			std::string VertexShaderPath = "Default";
			std::string PixelShaderPath = "Default";
			Components::GenericCamera* InitialCamera = nullptr;
		};
		//Renderer for 2D Components
		class SpriteRenderSystem : public Core::System<SpriteRenderSystem> {
		public:
			SpriteRenderSystem();
	
			bool Initialize(const SpriteRenderSystemDesc& desc);

			bool SetActiveCamera(Components::GenericCamera* Camera);
			//Update Functions
			void Update(Core::EntityManager &es, Core::EventManager &events, Core::TimeDelta dt) override;

		private:
			API::VertexShader mVShader;
			API::PixelShader mPShader;
			API::VertexBuffer mVertexBuffer;
			API::ConstantBuffer mSpriteColorBuffer;
			Components::GenericCamera* mCamera;
		};

	}
}

//Implementation (Required since EntityX and C++ Templates doesn't play nicely with DLL Export)
#define SpriteRenderSystemImpl
#include "SpriteRenderSystemImpl.inl"
#undef SpriteRenderSystemImpl