#pragma once
#include <NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/RenderTarget.h>
#include <Components/CameraComponent.h>

namespace Nuclear
{
	namespace Rendering
	{
		struct RenderingModuleDesc
		{
			Uint32 RTWidth;
			Uint32 RTHeight;


		};

		class NEAPI RenderingModule
		{
		public:
			static RenderingModule& GetInstance();

			RenderingModule(const RenderingModule&) = delete;
			RenderingModule& operator= (const RenderingModule) = delete;

			bool Initialize(const RenderingModuleDesc& desc);

			void Shutdown();

			void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight);

			void RenderFinalRT();

			void UpdateCameraCB(Components::CameraComponent* component);
			void UpdateCameraCB(const Components::CameraBuffer& bufferdata);

			Diligent::IBuffer* GetCameraCB();
			Diligent::IBuffer* GetAnimationCB();

			Graphics::RenderTarget& GetFinalRT();
			Graphics::RenderTarget& GetFinalDepthRT();

		protected:
			RenderingModuleDesc mDesc;

			//rendering constant buffers
			Diligent::RefCntAutoPtr<Diligent::IBuffer> mCameraCB;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> mAnimationCB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> pSceneToScreenPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pSceneToScreenSRB;

			Graphics::RenderTarget mFinalRT;
			Graphics::RenderTarget mFinalDepthRT;

			bool InitSceneToScreenPSO();

		private:
			RenderingModule();
		};

	}
}