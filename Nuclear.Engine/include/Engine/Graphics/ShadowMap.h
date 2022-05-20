#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace Nuclear
{
	namespace Systems {
		class LightingSystem;
	}

	namespace Graphics
	{

		enum class LightType {
			DirLight,
			SpotLight,
			PointLight
		};

		struct ShadowMapDesc
		{
			Uint32 mResolution = 1024;

		};

		class NEAPI ShadowMap
		{
		public:
			ShadowMap(const LightType& lighttype);
			~ShadowMap();

			bool isInitialized();

			ShadowMapDesc GetDesc() const;

			ITextureView* GetDSV();
			ITextureView* GetSRV();

		protected:
			friend class Systems::LightingSystem;

			void Initialize(const ShadowMapDesc& Desc);

		private:
			RefCntAutoPtr<ITextureView> mDSV;
			RefCntAutoPtr<ITextureView> mSRV;
			ShadowMapDesc mDesc;
			LightType mType;
			bool mInitialized = false;
		};
	}
}