#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Engine\Graphics\RenderTarget.h>

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

		class NEAPI ShadowMap : public RenderTarget
		{
		public:
			ShadowMap(const LightType& lighttype);
			~ShadowMap();

			bool isInitialized();

			//ShadowMapDesc GetDesc() const;

		protected:

			friend class Systems::LightingSystem;

			void Initialize(const ShadowMapDesc& Desc);

		private:
			
			ShadowMapDesc mDesc;
			LightType mType;
			bool mInitialized = false;
		};
	}
}