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

			void Initialize(Components::CameraComponent* Camera, Diligent::ITexture* data);
			//void Initialize(Components::CameraComponent* Camera, const std::array<LLGL::SrcImageDescriptor, 6>& data);
			void Initialize(Components::CameraComponent* Camera, const std::array<std::string, 6>& paths);

			void Release();
			void Render();

		private:
			Diligent::RefCntAutoPtr<Diligent::IShader> mVShader;
			Diligent::RefCntAutoPtr<Diligent::IShader> mPShader;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> mVBuffer;
			Diligent::RefCntAutoPtr<Diligent::ISampler> mSampler;
			//Diligent::RefCntAutoPtr<Diligent::DepthStencilStateDesc> mDepthState;
			Diligent::RefCntAutoPtr<Diligent::ITexture> mTexture;
			bool ReleaseTex = false;
		};
	}
}