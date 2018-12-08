#pragma once
#include <LLGL/LLGL.h>
#include <Base\NE_Common.h>
#include <Engine\Graphics\Shader.h>

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

			void Initialize(Components::CameraComponent* Camera, LLGL::Texture* data);
			void Initialize(Components::CameraComponent* Camera, const std::array<LLGL::SrcImageDescriptor, 6>& data);
			void Initialize(Components::CameraComponent* Camera, const std::array<std::string, 6>& paths);

			void Release();
			void Render();

		private:
			Graphics::Shader mVShader;
			Graphics::Shader mPShader;
			LLGL::Buffer* mVBuffer;
			LLGL::Sampler* mSampler;
			LLGL::DepthDescriptor mDepthState;
			LLGL::Texture* mTexture;
			bool ReleaseTex = false;
		};
	}
}