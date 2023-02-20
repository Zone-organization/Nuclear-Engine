///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-2021, Intel Corporation 
// 
// SPDX-License-Identifier: MIT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XeGTAO is based on GTAO/GTSO "Jimenez et al. / Practical Real-Time Strategies for Accurate Indirect Occlusion", 
// https://www.activision.com/cdn/research/Practical_Real_Time_Strategies_for_Accurate_Indirect_Occlusion_NEW%20VERSION_COLOR.pdf
// 
// Implementation:  Filip Strugar (filip.strugar@intel.com), Steve Mccalla <stephen.mccalla@intel.com>         (\_/)
// Version:         (see XeGTAO.h)                                                                            (='.'=)
// Details:         https://github.com/GameTechDev/XeGTAO                                                     (")_(")
//
// Version history: see XeGTAO.h
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Assets/Shader.h>
#include <Components/CameraComponent.h>

#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>

#include "XeGTAO.h"

namespace Nuclear
{
    namespace Rendering
    {
        class NEAPI GTAO
        {
        protected:
            mutable bool                                m_debugShowNormals = false;
            mutable bool                                m_debugShowBentNormals = false;
            mutable bool                                m_debugShowEdges = false;
            mutable bool                                m_debugShowGTAODebugViz = false;

            bool                                        m_use32bitDepth = false;
            bool                                        m_use16bitMath = true;
            bool                                        m_generateNormals = false;
            vaVector2i                                  m_size;

            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_workingDepths;
            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_workingDepthsMIPViews[XE_GTAO_DEPTH_MIP_LEVELS];
            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_workingAOTerm;            // AO alone or AO+BentNormals
            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_workingAOTermPong;        // same as ^, used for ping-ponging
            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_workingEdges;
            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_debugImage;
            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_workingNormals;

            Diligent::RefCntAutoPtr<Diligent::ITextureView>                       m_hilbertLUT;

            mutable XeGTAO::GTAOSettings                m_settings;

            bool                                        m_constantsMatchDefaults = false;        // just an optimization thing - see XE_GTAO_USE_DEFAULT_CONSTANTS

            bool                                        m_outputBentNormals = false;        // as given in the last Compute( ... ) call

            bool m_shadersDirty;

            // MSAA versions include 1-sample for non-MSAA
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_GenerateNormals;                        // optional screen space normal generation from depth (and results could be reused elsewhere)
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_PrefilterDepths16x16;                   // pass 1
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_GTAOLow;                                // pass 2 - low quality
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_GTAOMedium;                             // pass 2 - medium quality
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_GTAOHigh;                               // pass 2 - high quality
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_GTAOUltra;                              // pass 2 - ultra quality
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_DenoisePass;                            // pass 3 - one denoiser pass
            Diligent::RefCntAutoPtr<Diligent::IPipelineState>                 m_DenoiseLastPass;                        // pass 3 - last denoiser pass that outputs into the final output

            Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_constantBuffer;

            std::vector< std::pair< std::string, std::string > >       m_staticShaderMacros;

        protected:
            GTAO();

        public:
            virtual ~GTAO() {}

        public:
            void Compute(const Components::CameraComponent& cameraBase, bool usingTAA, bool outputBentNormals, const Diligent::RefCntAutoPtr<Diligent::ITextureView>& outputAO, const Diligent::RefCntAutoPtr<Diligent::ITextureView>& inputDepth, const Diligent::RefCntAutoPtr<Diligent::ITextureView>& inputNormals);

        public:
            XeGTAO::GTAOSettings& Settings() { return m_settings; }
            bool& Use16bitMath() { return m_use16bitMath; }

            bool& DebugShowNormals() { return m_debugShowNormals; }
            bool& DebugShowEdges() { return m_debugShowEdges; }
            bool& DebugShowBentNormals() { return m_debugShowBentNormals; }
            bool                                        DebugShowImage() { return m_debugShowNormals || m_debugShowBentNormals || m_debugShowEdges; }
            const Diligent::RefCntAutoPtr<Diligent::ITextureView>                 DebugImage() { return m_debugImage; }


        public:
            virtual void                                UIPanelTick();

        private:
            bool                                        UpdateTexturesAndShaders(int width, int height);
            void                                        UpdateConstants(vaRenderDeviceContext& renderContext, const vaMatrix4x4& projMatrix, bool usingTAA);
        };

    }
}