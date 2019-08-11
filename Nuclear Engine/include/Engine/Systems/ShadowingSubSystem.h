/*
Modified Work from
https://github.com/DiligentGraphics/DiligentFX/blob/master/Components/interface/ShadowMapManager.h

 *  Copyright 2015-2019 Egor Yusov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF ANY PROPRIETARY RIGHTS.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */
#include <Base/NE_Common.h>
#include <vector>
#include <array>

#include <Diligent/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Common/interface/BasicMath.h>

#define MAX_CASCADES 8
#define SHADOW_MODE_PCF 1
#define SHADOW_MODE_VSM 2
#define SHADOW_MODE_EVSM2 3
#define SHADOW_MODE_EVSM4 4
#ifndef SHADOW_MODE
#   define SHADOW_MODE SHADOW_MODE_PCF
#endif

namespace NuclearEngine
{

	namespace Shadowing
	{
		struct CascadeAttribs
		{
			float4 f4LightSpaceScale;
			float4 f4LightSpaceScaledBias;
			float4 f4StartEndZ;

			// Cascade margin in light projection space ([-1, +1] x [-1, +1] x [-1(GL) or 0, +1])
			float4 f4MarginProjSpace;
		};

		struct ShadowMapAttribs
		{
			float4x4 mWorldToLightViewT;
			CascadeAttribs Cascades[MAX_CASCADES];

			float4x4 mWorldToShadowMapUVDepthT[MAX_CASCADES];
			float fCascadeCamSpaceZEnd[MAX_CASCADES];
			float4 f4ShadowMapDim;    // Width, Height, 1/Width, 1/Height

			// Number of shadow cascades
			int   iNumCascades                  = 0;
			float fNumCascades                  = 0;
			// Do not use bool, because sizeof(bool)==1 !
			int32_t  bVisualizeCascades            = 0;
			int32_t  bVisualizeShadowing           = 0;

			float fReceiverPlaneDepthBiasClamp  = 10;
			float fFixedDepthBias               = 1e-5f;
			float fCascadeTransitionRegion      = 0.1f;
			int   iMaxAnisotropy                = 4;

			float fVSMBias                      = 1e-4f;
			float fVSMLightBleedingReduction    = 0;
			float fEVSMPositiveExponent         = 40;
			float fEVSMNegativeExponent         = 5;

			int32_t  bIs32BitEVSM                  = 1;
			int   iFixedFilterSize              = 3; // 3x3 filter
			float fFilterWorldSize              = 0;
			bool  fDummy;
		};
	}

	namespace Systems
	{
		class NEAPI ShadowingSubSystem
		{
		public:
			ShadowingSubSystem();

			/// Shadow map manager initialization info
			struct InitInfo
			{
				/// Shadow map format. This parameter must not be TEX_FORMAT_UNKNOWN.
				TEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;

				/// Shadow map resolution, must not be 0.
				Uint32         Resolution = 0;

				/// Number of shadow cascades, must not be 0.
				Uint32         NumCascades = 0;

				/// Shadow mode (see SHADOW_MODE_* defines in BasicStructures.fxh), must not be 0.
				int            ShadowMode = 0;

				/// Wether to use 32-bit or 16-bit filterable textures
				bool           Is32BitFilterableFmt = false;

				/// Optional comparison sampler to be set in the shadow map resource view
				ISampler* pComparisonSampler = nullptr;

				/// Optional sampler to be set in the filterable shadow map representation
				ISampler* pFilterableShadowMapSampler = nullptr;
			};
			void Initialize(IRenderDevice* pDevice, const InitInfo& initInfo);

			ITextureView* GetSRV() { return m_pShadowMapSRV; }
			ITextureView* GetCascadeDSV(Uint32 Cascade) { return m_pShadowMapDSVs[Cascade]; }
			ITextureView* GetFilterableSRV() { return m_pFilterableShadowMapSRV; }

			struct DistributeCascadeInfo
			{
				/// Pointer to camera view matrix, must not be null.
				const float4x4* pCameraView = nullptr;

				/// Pointer to camera world matrix.
				const float4x4* pCameraWorld = nullptr;

				/// Pointer to camera projection matrix, must not be null.
				const float4x4* pCameraProj = nullptr;

				/// Pointer to light direction, must not be null.
				const float3* pLightDir = nullptr;

				/// Wether to snap cascades to texels in light view space
				bool               SnapCascades = true;

				/// Wether to stabilize cascade extents in light view space
				bool               StabilizeExtents = true;

				/// Wether to use same extents for X and Y axis. Enabled automatically if StabilizeExtents == true
				bool               EqualizeExtents = true;

				/// Cascade partitioning factor that defines the ratio between fully linear (0.0) and 
				/// fully logarithmic (1.0) partitioning.
				float              fPartitioningFactor = 0.95f;

				/// Callback that allows the application to adjust z range of every cascade.
				/// The callback is also called with cascade value -1 to adjust that entire camera range.
				std::function<void(int, float&, float&)> AdjustCascadeRange;
			};

			struct CascadeTransforms
			{
				float4x4 Proj;
				float4x4 WorldToLightProjSpace;
			};

			void DistributeCascades(const DistributeCascadeInfo& Info,
				Shadowing::ShadowMapAttribs& shadowMapAttribs);

			void ConvertToFilterable(IDeviceContext* pCtx, const Shadowing::ShadowMapAttribs& ShadowAttribs);

			const CascadeTransforms& GetCascadeTranform(Uint32 Cascade) const { return m_CascadeTransforms[Cascade]; }

		private:
			void InitializeConversionTechniques(TEXTURE_FORMAT FilterableShadowMapFmt);
			void InitializeResourceBindings();

			int                                      m_ShadowMode = 0;
			RefCntAutoPtr<IRenderDevice>             m_pDevice;
			RefCntAutoPtr<ITextureView>              m_pShadowMapSRV;
			std::vector<RefCntAutoPtr<ITextureView>> m_pShadowMapDSVs;
			RefCntAutoPtr<ITextureView>              m_pFilterableShadowMapSRV;
			std::vector<RefCntAutoPtr<ITextureView>> m_pFilterableShadowMapRTVs;
			RefCntAutoPtr<ITextureView>              m_pIntermediateSRV;
			RefCntAutoPtr<ITextureView>              m_pIntermediateRTV;
			RefCntAutoPtr<IBuffer>                   m_pConversionAttribsBuffer;
			std::vector<CascadeTransforms>           m_CascadeTransforms;
			struct ShadowConversionTechnique
			{
				RefCntAutoPtr<IPipelineState>         PSO;
				RefCntAutoPtr<IShaderResourceBinding> SRB;
			};
			std::array<ShadowConversionTechnique, SHADOW_MODE_EVSM4 + 1> m_ConversionTech;
			ShadowConversionTechnique m_BlurVertTech;
		};

	}
}