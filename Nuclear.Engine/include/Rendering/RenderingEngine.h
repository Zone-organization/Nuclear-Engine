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
		struct RenderingEngineDesc
		{
			Uint32 RTWidth;
			Uint32 RTHeight;


		};

		class NEAPI RenderingEngine
		{
		public:
			static RenderingEngine& GetInstance();

			RenderingEngine(const RenderingEngine&) = delete;
			RenderingEngine& operator= (const RenderingEngine) = delete;

			bool Initialize(const RenderingEngineDesc& desc);

			void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight);

			void RenderFinalRT();

			void UpdateCameraCB(Components::CameraComponent* component);
			void UpdateCameraCB(const Components::CameraBuffer& bufferdata);

			Diligent::IBuffer* GetCameraCB();
			Diligent::IBuffer* GetAnimationCB();

			Graphics::RenderTarget& GetFinalRT();
			Graphics::RenderTarget& GetFinalDepthRT();

		protected:
			RenderingEngineDesc mDesc;

			//rendering constant buffers
			Diligent::RefCntAutoPtr<Diligent::IBuffer> mCameraCB;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> mAnimationCB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> pSceneToScreenPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pSceneToScreenSRB;

			Graphics::RenderTarget mFinalRT;
			Graphics::RenderTarget mFinalDepthRT;

			bool InitSceneToScreenPSO();

		private:
			RenderingEngine();
		};

	}
}