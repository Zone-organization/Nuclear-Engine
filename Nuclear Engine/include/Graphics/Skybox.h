#pragma once
#include <Graphics/API/VertexBuffer.h>
#include <Graphics/API/IndexBuffer.h>
#include <Graphics/API/Texture.h>
#include <Graphics/API/Sampler.h>
#include <Graphics/API/DepthStencilState.h>
#include <Core/FileSystem.h>
#include <Components\CameraComponent.h>

namespace NuclearEngine
{
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