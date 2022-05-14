#pragma once
#include <Engine\Components\DirLightComponent.h>
#include <Engine\Components\PointLightComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine\Rendering\ShadingModel.h>
#include <vector>
#include <Engine\ECS\System.h>

namespace Nuclear
{
	namespace Systems
	{

		class CameraSystem;
		struct ShadowMapInfo
		{
			Uint32 mResolution = 1024;
			TEXTURE_FORMAT mFormat = TEX_FORMAT_D32_FLOAT;
		};

		struct LightingSystemDesc
		{
			bool EnableShadows = false;
			ShadowMapInfo mSpotLightShadowMapInfo;
		};
		class NEAPI LightingSystem : public ECS::System<LightingSystem>
		{
		public:
			LightingSystem(const LightingSystemDesc& desc = LightingSystemDesc());

			bool RequiresBaking();

			void Bake();

			void Update(ECS::TimeDelta dt) override;

			//Update Functions
			void UpdateBuffer(const Math::Vector4& CameraPos);

			//Debug Only
			bool VisualizePointLightsPositions = false;

			IBuffer* GetLightCB();

			size_t GetDirLightsNum();
			size_t GetPointLightsNum();
			size_t GetSpotLightsNum();

		private:
			void BakeBuffer();

			size_t Baked_DirLights_Size = 0;
			size_t Baked_PointLights_Size = 0;
			size_t Baked_SpotLights_Size = 0;
			size_t NE_Light_CB_Size;
			size_t NUM_OF_LIGHT_VECS;

			bool HasbeenBakedBefore = false;

			RefCntAutoPtr<IBuffer> mPSLightCB;

			std::vector<Components::DirLightComponent*> DirLights;
			std::vector<Components::PointLightComponent*> PointLights;
			std::vector<Components::SpotLightComponent*> SpotLights;

			LightingSystemDesc mDesc;

			//Shadows
			RefCntAutoPtr<ITextureView> mShadowMapDSV;
			RefCntAutoPtr<ITextureView> mShadowMapSRV;

			RefCntAutoPtr<IPipelineState> mSpotShadowMapDepthPSO;
			RefCntAutoPtr<IShaderResourceBinding> mSpotShadowMapDepthSRB;
			RefCntAutoPtr<IBuffer> pSpotLightInfoCB;

			void RenderMeshForDepthPass(Assets::Mesh* mesh);
			void SpotLightShadowDepthPass(const Components::SpotLightComponent& spotlight);

			void InitSpotLightShadowPSO();
			void InitSpotLightShadowMapDSV();
		};

	}
}