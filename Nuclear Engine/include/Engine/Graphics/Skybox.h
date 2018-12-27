#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DepthStencilState.h>

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

			//void Initialize(Components::CameraComponent* Camera, ITexture* data);
			//void Initialize(Components::CameraComponent* Camera, const std::array<LLGL::SrcImageDescriptor, 6>& data);
			void Initialize(Components::CameraComponent* Camera, const std::array<std::string, 6>& paths);

			void Release();
			void Render();

		private:
			RefCntAutoPtr<IShader> mVShader;
			RefCntAutoPtr<IShader> mPShader;
			RefCntAutoPtr<IBuffer> mVBuffer;
			RefCntAutoPtr<ISampler> mSampler;
			//RefCntAutoPtr<DepthStencilStateDesc> mDepthState;
			RefCntAutoPtr<ITexture> mTexture;
			bool ReleaseTex = false;
		};
	}
}