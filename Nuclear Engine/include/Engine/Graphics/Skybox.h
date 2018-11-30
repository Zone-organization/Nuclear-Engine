#pragma once
#include <Engine/Graphics/API/VertexBuffer.h>
#include <Engine/Graphics/API/Texture.h>
#include <Engine/Graphics/API/Sampler.h>
#include <Engine/Graphics/API/DepthStencilState.h>
#include <Engine/Graphics/API/VertexShader.h>
#include <Engine/Graphics/API/PixelShader.h>


namespace NuclearEngine
{
	namespace Components
	{
		class CameraComponent;
	}
	namespace Graphics
	{			
		class NEAPI Skybox
		{
		public:
			Skybox();
			~Skybox();

			void Initialize(Components::CameraComponent* Camera, Graphics::API::Texture data);
			void Initialize(Components::CameraComponent* Camera, const std::array<Graphics::API::Texture_Data, 6>& data);
			void Initialize(Components::CameraComponent* Camera, const std::array<std::string, 6>& paths);

			void Release();
			void Render();

		private:
			Graphics::API::VertexShader mVShader;
			Graphics::API::PixelShader mPShader;
			Graphics::API::VertexBuffer mVBuffer;
			Graphics::API::Sampler mSampler;
			Graphics::API::DepthStencilState mDSState;
			Graphics::API::Texture mTexture;
			bool ReleaseTex = false;
		};
	}
}