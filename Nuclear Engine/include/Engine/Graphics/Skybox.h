#pragma once
#include <LLGL/LLGL.h>
#include <Base\NE_Common.h>

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
			void Initialize(Components::CameraComponent* Camera, const std::array<LLGL::Texture_Data, 6>& data);
			void Initialize(Components::CameraComponent* Camera, const std::array<std::string, 6>& paths);

			void Release();
			void Render();

		private:
			LLGL::Shader* mVShader;
			LLGL::Shader* mPShader;
			LLGL::Buffer* mVBuffer;
			LLGL::Sampler mSampler;
			LLGL::DepthStencilState mDSState;
			LLGL::Texture* mTexture;
			bool ReleaseTex = false;
		};
	}
}