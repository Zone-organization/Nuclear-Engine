#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <ThirdParty/ShadowMapManager/ShadowMapManager.h>

namespace NuclearEngine
{
	namespace Systems
	{
		class NEAPI ShadowSystem
		{
		public:
		
            void CreateShadowMap();
            void RenderShadowMap();
		private:
            void InitializeResourceBindings();
            struct ShadowSettings
            {
                bool           SnapCascades = true;
                bool           StabilizeExtents = true;
                bool           EqualizeExtents = true;
                bool           SearchBestCascade = true;
                bool           FilterAcrossCascades = true;
                int            Resolution = 2048;
                float          PartitioningFactor = 0.95f;
                TEXTURE_FORMAT Format = TEX_FORMAT_D16_UNORM;
                int            iShadowMode = SHADOW_MODE_PCF;

                bool Is32BitFilterableFmt = true;
            } m_ShadowSettings;

            LightAttribs      m_LightAttribs;

            ShadowMapManager m_ShadowMapMgr;

            RefCntAutoPtr<IBuffer>                             m_CameraAttribsCB;
            RefCntAutoPtr<IBuffer>                             m_LightAttribsCB;
            std::vector<Uint32>                                m_PSOIndex;
            std::vector<RefCntAutoPtr<IPipelineState>>         m_RenderMeshPSO;
            std::vector<RefCntAutoPtr<IPipelineState>>         m_RenderMeshShadowPSO;
            std::vector<RefCntAutoPtr<IShaderResourceBinding>> m_SRBs;
            std::vector<RefCntAutoPtr<IShaderResourceBinding>> m_ShadowSRBs;

            RefCntAutoPtr<ISampler> m_pComparisonSampler;
            RefCntAutoPtr<ISampler> m_pFilterableShadowMapSampler;

			bool HasbeenBakedBefore = false;

		};

	}
}