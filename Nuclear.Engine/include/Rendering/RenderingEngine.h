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

			void Initialize(const RenderingEngineDesc& desc);

			void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight);

			void RenderFinalRT();

			void UpdateCameraCB(Components::CameraComponent* component);
			void UpdateCameraCB(const Components::CameraBuffer& bufferdata);

			IBuffer* GetCameraCB();
			IBuffer* GetAnimationCB();

			Graphics::RenderTarget& GetFinalRT();
			Graphics::RenderTarget& GetFinalDepthRT();

		protected:
			RenderingEngineDesc mDesc;

			//rendering constant buffers
			RefCntAutoPtr<IBuffer> mCameraCB;
			RefCntAutoPtr<IBuffer> mAnimationCB;

			RefCntAutoPtr<IPipelineState> pSceneToScreenPSO;
			RefCntAutoPtr<IShaderResourceBinding> pSceneToScreenSRB;

			Graphics::RenderTarget mFinalRT;
			Graphics::RenderTarget mFinalDepthRT;

			void InitSceneToScreenPSO();

		private:
			RenderingEngine();
		};

	}
}