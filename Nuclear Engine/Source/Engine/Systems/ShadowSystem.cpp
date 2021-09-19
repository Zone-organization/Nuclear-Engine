#include "Engine\Systems\ShadowSystem.h"
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Systems
	{
		void ShadowSystem::CreateShadowMap()
		{
            if (m_ShadowSettings.Resolution >= 2048)
                m_LightAttribs.ShadowAttribs.fFixedDepthBias = 0.0025f;
            else if (m_ShadowSettings.Resolution >= 1024)
                m_LightAttribs.ShadowAttribs.fFixedDepthBias = 0.005f;
            else
                m_LightAttribs.ShadowAttribs.fFixedDepthBias = 0.0075f;

            ShadowMapManager::InitInfo SMMgrInitInfo;
            SMMgrInitInfo.Format = m_ShadowSettings.Format;
            SMMgrInitInfo.Resolution = m_ShadowSettings.Resolution;
            SMMgrInitInfo.NumCascades = static_cast<Uint32>(m_LightAttribs.ShadowAttribs.iNumCascades);
            SMMgrInitInfo.ShadowMode = m_ShadowSettings.iShadowMode;
            SMMgrInitInfo.Is32BitFilterableFmt = m_ShadowSettings.Is32BitFilterableFmt;

            if (!m_pComparisonSampler)
            {
                SamplerDesc ComparsionSampler;
                ComparsionSampler.ComparisonFunc = COMPARISON_FUNC_LESS;
                // Note: anisotropic filtering requires SampleGrad to fix artifacts at
                // cascade boundaries
                ComparsionSampler.MinFilter = FILTER_TYPE_COMPARISON_LINEAR;
                ComparsionSampler.MagFilter = FILTER_TYPE_COMPARISON_LINEAR;
                ComparsionSampler.MipFilter = FILTER_TYPE_COMPARISON_LINEAR;
                Graphics::Context::GetDevice()->CreateSampler(ComparsionSampler, &m_pComparisonSampler);
            }
            SMMgrInitInfo.pComparisonSampler = m_pComparisonSampler;

            if (!m_pFilterableShadowMapSampler)
            {
                SamplerDesc SamplerDesc;
                SamplerDesc.MinFilter = FILTER_TYPE_ANISOTROPIC;
                SamplerDesc.MagFilter = FILTER_TYPE_ANISOTROPIC;
                SamplerDesc.MipFilter = FILTER_TYPE_ANISOTROPIC;
                SamplerDesc.MaxAnisotropy = m_LightAttribs.ShadowAttribs.iMaxAnisotropy;
                Graphics::Context::GetDevice()->CreateSampler(SamplerDesc, &m_pFilterableShadowMapSampler);
            }
            SMMgrInitInfo.pFilterableShadowMapSampler = m_pFilterableShadowMapSampler;

            m_ShadowMapMgr.Initialize(Graphics::Context::GetDevice(), SMMgrInitInfo);

            InitializeResourceBindings();
		}
        void ShadowSystem::RenderShadowMap()
        {
            auto iNumShadowCascades = m_LightAttribs.ShadowAttribs.iNumCascades;
            for (int iCascade = 0; iCascade < iNumShadowCascades; ++iCascade)
            {
                const auto CascadeProjMatr = m_ShadowMapMgr.GetCascadeTranform(iCascade).Proj;

                auto WorldToLightViewSpaceMatr = m_LightAttribs.ShadowAttribs.mWorldToLightViewT.Transpose();
                auto WorldToLightProjSpaceMatr = WorldToLightViewSpaceMatr * CascadeProjMatr;

                CameraAttribs ShadowCameraAttribs = {};

                ShadowCameraAttribs.mViewT = m_LightAttribs.ShadowAttribs.mWorldToLightViewT;
                ShadowCameraAttribs.mProjT = CascadeProjMatr.Transpose();
                ShadowCameraAttribs.mViewProjT = WorldToLightProjSpaceMatr.Transpose();

                ShadowCameraAttribs.f4ViewportSize.x = static_cast<float>(m_ShadowSettings.Resolution);
                ShadowCameraAttribs.f4ViewportSize.y = static_cast<float>(m_ShadowSettings.Resolution);
                ShadowCameraAttribs.f4ViewportSize.z = 1.f / ShadowCameraAttribs.f4ViewportSize.x;
                ShadowCameraAttribs.f4ViewportSize.w = 1.f / ShadowCameraAttribs.f4ViewportSize.y;

                //{
                //    MapHelper<CameraAttribs> CameraData(Graphics::Context::GetContext(), m_CameraAttribsCB, MAP_WRITE, MAP_FLAG_DISCARD);
                //    *CameraData = ShadowCameraAttribs;
                //}

                //auto* pCascadeDSV = m_ShadowMapMgr.GetCascadeDSV(iCascade);
                //Graphics::Context::GetContext()->SetRenderTargets(0, nullptr, pCascadeDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                //Graphics::Context::GetContext()->ClearDepthStencil(pCascadeDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                //ViewFrustumExt Frutstum;
                //ExtractViewFrustumPlanesFromMatrix(WorldToLightProjSpaceMatr, Frutstum, Graphics::Context::GetDevice()->GetDeviceInfo().IsGLDevice());

                ////if (iCascade == 0)
                //DrawMesh(Graphics::Context::GetContext(), true, Frutstum);
            }

            if (m_ShadowSettings.iShadowMode > SHADOW_MODE_PCF)
                m_ShadowMapMgr.ConvertToFilterable(Graphics::Context::GetContext(), m_LightAttribs.ShadowAttribs);
        }
        void ShadowSystem::InitializeResourceBindings()
        {
            m_SRBs.clear();
            m_ShadowSRBs.clear();
            //m_SRBs.resize(m_Mesh.GetNumMaterials());
            //m_ShadowSRBs.resize(m_Mesh.GetNumMaterials());
            //for (Uint32 mat = 0; mat < m_Mesh.GetNumMaterials(); ++mat)
            //{
            //    {
            //        const auto& Mat = m_Mesh.GetMaterial(mat);

            //        RefCntAutoPtr<IShaderResourceBinding> pSRB;
            //        m_RenderMeshPSO[0]->CreateShaderResourceBinding(&pSRB, true);
            //        VERIFY(Mat.pDiffuseRV != nullptr, "Material must have diffuse color texture");
            //        pSRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_tex2DDiffuse")->Set(Mat.pDiffuseRV);
            //        if (m_ShadowSettings.iShadowMode == SHADOW_MODE_PCF)
            //        {
            //            pSRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_tex2DShadowMap")->Set(m_ShadowMapMgr.GetSRV());
            //        }
            //        else
            //        {
            //            pSRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_tex2DFilterableShadowMap")->Set(m_ShadowMapMgr.GetFilterableSRV());
            //        }
            //        m_SRBs[mat] = std::move(pSRB);
            //    }

            //    {
            //        RefCntAutoPtr<IShaderResourceBinding> pShadowSRB;
            //        m_RenderMeshShadowPSO[0]->CreateShaderResourceBinding(&pShadowSRB, true);
            //        m_ShadowSRBs[mat] = std::move(pShadowSRB);
            //    }
            //}
        }
	}
}