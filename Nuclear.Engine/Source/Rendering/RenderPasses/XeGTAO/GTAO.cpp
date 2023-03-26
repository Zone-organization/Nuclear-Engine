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

#include "GTAO.h"

#include <Graphics/Context.h>
#include <Graphics/ImGui.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Platform\FileSystem.h>

#define VA_IMGUI_INTEGRATION_ENABLED
namespace Nuclear
{
    namespace Rendering
    {
        using namespace Diligent;
        Diligent::RefCntAutoPtr<Diligent::ITexture> CreateTexture(TEXTURE_FORMAT format, Uint32 w, Uint32 h, Uint32 mipLevels, Uint32 arraySize, Uint32 sampleCount, BIND_FLAGS bind_flags, const char* name)
        {
            Diligent::TextureDesc mdesc;
            mdesc.BindFlags = bind_flags;
            mdesc.Name = name;
            Diligent::ITexture *result;
            return Diligent::RefCntAutoPtr<Diligent::ITexture>(result);
        }

        GTAO::GTAO()
        {
            {
                BufferDesc CBDesc;
                CBDesc.Name = "GTAOConstants";
                CBDesc.Size = sizeof(XeGTAO::GTAOConstants);
                CBDesc.Usage = USAGE_DYNAMIC;
                CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
                CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
                Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &m_constantBuffer);
            }

            // Hilbert look-up texture! It's a 64 x 64 Uint16 texture generated using XeGTAO::HilbertIndex
            {
                Uint16* data = new Uint16[64 * 64];
                for (int x = 0; x < 64; x++)
                    for (int y = 0; y < 64; y++)
                    {
                        Uint32 r2index = XeGTAO::HilbertIndex(x, y);
                        assert(r2index < 65536);
                        data[x + 64 * y] = (Uint16)r2index;
                    }
      ///   //       m_hilbertLUT = CreateTexture(TEX_FORMAT_R16_UINT, 64, 64, 1, 1, 1, BIND_SHADER_RESOURCE, vaResourceAccessFlags::Default, TEX_FORMAT_Automatic, TEX_FORMAT_Automatic, TEX_FORMAT_Automatic, TEX_FORMAT_Automatic, vaTextureFlags::None, vaTextureContentsType::GenericLinear,
       ////             data, 64 * 2);
                delete[] data;
            }
        }

        void GTAO::UIPanelTick()
        {
#ifdef VA_IMGUI_INTEGRATION_ENABLED

          /// VA_GENERIC_RAII_SCOPE(ImGui::PushItemWidth(120.0f); , ImGui::PopItemWidth(); );

            {
                ImGui::Separator();

                if (m_constantsMatchDefaults)
                    ImGui::TextColored({ 0.5f, 1.0f, 0.5f, 1.0f }, "Heuristics settings match defaults, shader will be faster");
                else
                    ImGui::TextColored({ 1, 0.5f, 0.5f, 1.0f }, "Heuristics settings don't match defaults, shader will be slower");

                ImGui::Separator();
                ImGui::Text("External settings:");

                ImGui::Checkbox("Generate normals from depth", &m_generateNormals);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Viewspace normals can be either supplied (recommended) or generated from the depth buffer (lower performance and usually lower quality).");

                ImGui::Checkbox("Use 32bit working depth buffer", &m_use32bitDepth);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Working depth buffer can be 16 bit (faster but slightly less quality) or 32 bit (slightly higher quality, slower). 32bit buffer is recommended if generating normals from depths.");

                m_use16bitMath &= !m_use32bitDepth; // FP16 math not compatible with 32bit depths.

                ImGui::Checkbox("Use 16bit shader math", &m_use16bitMath);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Faster on some GPUs, with some (limited) quality degradation. Not compatible with 32bit depths.");
            }

            if (ImGui::CollapsingHeader("Development and debugging", 0))
            {
                {
                    ImGui::Checkbox("Debug: Show GTAO debug viz", &m_debugShowGTAODebugViz);
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show GTAO debug visualization");

                    if (ImGui::Checkbox("Debug: Show normals", &m_debugShowNormals))
                    {
                        m_debugShowEdges &= !m_debugShowNormals;
                        m_debugShowBentNormals &= !m_debugShowNormals;
                    }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show screen space normals");

                    if (m_outputBentNormals && ImGui::Checkbox("Debug: Show output bent normals", &m_debugShowBentNormals))
                    {
                        m_debugShowEdges &= !m_debugShowBentNormals;
                        m_debugShowNormals &= !m_debugShowBentNormals;
                    }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show generated screen space bent normals");

                    if (ImGui::Checkbox("Debug: Show denoising edges", &m_debugShowEdges))
                    {
                        m_debugShowNormals &= !m_debugShowEdges;
                        m_debugShowBentNormals &= !m_debugShowEdges;
                    }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show edges not crossed by denoising blur");

//#ifndef VA_GTAO_SAMPLE
//                    ImGui::Text("Dump DXIL disassembly to file:");
//                    if (ImGui::Button("MainPass-High"))
//                        m_CSGTAOHigh->DumpDisassembly("XeGTAO_MainPass.txt");
//                    ImGui::SameLine();
//                    if (ImGui::Button("Denoise"))
//                        m_CSDenoisePass->DumpDisassembly("XeGTAO_Denoise.txt");
//#endif
                }
            }

#endif
        }

        bool GTAO::UpdateTexturesAndShaders(int width, int height)
        {
            if (!m_generateNormals)
                m_workingNormals = nullptr;

            bool hadChanges = false;
            std::vector< std::pair< std::string, std::string > > newShaderMacros;

            // global shader switches - can be omitted and GTAO will default to most common use case
            if (m_use32bitDepth)
                newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_FP32_DEPTHS", ""));
            m_use16bitMath &= !m_use32bitDepth; // FP16 math not compatible with 32bit depths.
            newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_USE_HALF_FLOAT_PRECISION", (m_use16bitMath) ? ("1") : ("0")));

            if (m_outputBentNormals)
                newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_COMPUTE_BENT_NORMALS", ""));

            // debugging switches
            if (m_debugShowGTAODebugViz)
                newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_SHOW_DEBUG_VIZ", ""));
            if (m_debugShowNormals)
                newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_SHOW_NORMALS", ""));
            if (m_debugShowBentNormals)
                newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_SHOW_BENT_NORMALS", ""));
            if (m_debugShowEdges)
                newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_SHOW_EDGES", ""));

            if (m_hilbertLUT != nullptr)
                newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_HILBERT_LUT_AVAILABLE", ""));

            m_constantsMatchDefaults =
                (m_settings.RadiusMultiplier == XE_GTAO_DEFAULT_RADIUS_MULTIPLIER)
                && (m_settings.SampleDistributionPower == XE_GTAO_DEFAULT_SAMPLE_DISTRIBUTION_POWER)
                && (m_settings.FalloffRange == XE_GTAO_DEFAULT_FALLOFF_RANGE)
                && (m_settings.ThinOccluderCompensation == XE_GTAO_DEFAULT_THIN_OCCLUDER_COMPENSATION)
                && (m_settings.FinalValuePower == XE_GTAO_DEFAULT_FINAL_VALUE_POWER)
                ;

            newShaderMacros.push_back(std::pair<std::string, std::string>("XE_GTAO_USE_DEFAULT_CONSTANTS", (m_constantsMatchDefaults) ? ("1") : ("0")));

            if (newShaderMacros != m_staticShaderMacros)
            {
                m_staticShaderMacros = newShaderMacros;
                m_shadersDirty = true;
            }
            std::set<std::string> shadermacros;
            for (auto& i : m_staticShaderMacros)
            {
                shadermacros.insert(i.first + " " + i.second);
            }

            std::string ShaderSource = Platform::FileSystem::GetInstance().LoadShader("@NuclearAssets@/Shaders/GTAO.hlsl", shadermacros, std::set<std::string>(), true);

            auto CreateGTAOShader = [&shadermacros, &ShaderSource](const std::string& entrypoint)
            {
                GTAOShader result;

                Diligent::RefCntAutoPtr<Diligent::IShader> shader;
                Graphics::ShaderObjectCreationDesc shader_desc;
                shader_desc.mType = SHADER_TYPE_COMPUTE;
                shader_desc.mSource = ShaderSource;
                shader_desc.mEntrypoint = shader_desc.mName = entrypoint;
                shader_desc.mDefines = shadermacros;
                shader_desc.mUseCombinedTextureSamplers = false;

                Graphics::GraphicsEngine::GetInstance().CreateShader(shader.RawDblPtr(), shader_desc);

                for (Uint32 i = 0; i < shader->GetResourceCount(); i++)
                {
                    Diligent::ShaderResourceDesc RsrcDesc;
                    shader->GetResourceDesc(i, RsrcDesc);

                    if (RsrcDesc.Type == SHADER_RESOURCE_TYPE_SAMPLER)
                    {
                        std::string name = RsrcDesc.Name;
                        
                    }
                }

                ComputePipelineStateCreateInfo PSOCreateInfo;
                PipelineStateDesc& PSODesc = PSOCreateInfo.PSODesc;

                PSOCreateInfo.pCS = shader;

                PSOCreateInfo;

                return result;

            };

               if (m_shadersDirty)
               {
                   m_shadersDirty = false;

                   // to allow parallel background compilation but still ensure they're all compiled after this function
                 //  std::vector<shared_ptr<vaShader>> allShaders;

                   m_PrefilterDepths16x16 = CreateGTAOShader("CSPrefilterDepths16x16");
                   m_GTAOLow = CreateGTAOShader("CSGTAOLow");
                   m_GTAOMedium = CreateGTAOShader("CSGTAOMedium");
                   m_GTAOHigh = CreateGTAOShader("CSGTAOHigh");
                   m_GTAOUltra = CreateGTAOShader("CSGTAOUltra");
                   m_DenoisePass = CreateGTAOShader("CSDenoisePass");
                   m_DenoiseLastPass = CreateGTAOShader("CSDenoiseLastPass");
                   m_GenerateNormals = CreateGTAOShader("CSGenerateNormals");

                   // wait until shaders are compiled! this allows for parallel compilation but ensures all are compiled after this point
                   //for (auto sh : allShaders) sh->WaitFinishIfBackgroundCreateActive();
                   //TODO: Parallel shader creation...

                   hadChanges = true;
               }

            bool needsUpdate = false;

            needsUpdate |= (m_size.x != width) || (m_size.y != height);
            needsUpdate |= (m_workingNormals == nullptr) && m_generateNormals;

            TEXTURE_FORMAT requiredDepthFormat = (m_use32bitDepth) ? (TEX_FORMAT_R32_FLOAT) : (TEX_FORMAT_R16_FLOAT);
            needsUpdate |= m_workingDepths == nullptr || m_workingDepths->GetDesc().Format != requiredDepthFormat;
            TEXTURE_FORMAT requiredAOTermFormat = (m_outputBentNormals) ? (TEX_FORMAT_R32_UINT) : (TEX_FORMAT_R8_UINT);
            needsUpdate |= m_workingAOTerm == nullptr || m_workingAOTerm->GetDesc().Format != requiredAOTermFormat;

            m_size.x = width;
            m_size.y = height;

            if (needsUpdate)
            {
                hadChanges = true;

                m_debugImage = CreateTexture(TEX_FORMAT_R11G11B10_FLOAT, m_size.x, m_size.y, 1, 1, 1, BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS, "XeGTAO_DebugImage");
                m_workingDepths = CreateTexture(requiredDepthFormat, m_size.x, m_size.y, XE_GTAO_DEPTH_MIP_LEVELS, 1, 1, BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS, "XeGTAO_WorkingDepths");
                for (int mip = 0; mip < XE_GTAO_DEPTH_MIP_LEVELS; mip++)
                {
                    TextureViewDesc viewdesc;
                   // viewdesc.


                    m_workingDepths->CreateView(viewdesc, &m_workingDepthsMIPViews[mip]);
      /////              m_workingDepthsMIPViews[mip] = vaTexture::CreateView(m_workingDepths, m_workingDepths->GetBindSupportFlags(), TEX_FORMAT_Automatic, TEX_FORMAT_Automatic, TEX_FORMAT_Automatic, TEX_FORMAT_Automatic, vaTextureFlags::None, mip, 1);
                }
                m_workingEdges = CreateTexture(TEX_FORMAT_R8_UNORM, m_size.x, m_size.y, 1, 1, 1, BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS, "XeGTA_WorkingEdges");
                m_workingAOTerm = CreateTexture(requiredAOTermFormat, m_size.x, m_size.y, 1, 1, 1, BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS, "XeGTAO_WorkingAOTerm1");
                m_workingAOTermPong = CreateTexture(requiredAOTermFormat, m_size.x, m_size.y, 1, 1, 1, BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS, "XeGTAO_WorkingAOTerm2");

                if (m_generateNormals)
                {
                    m_workingNormals = CreateTexture(TEX_FORMAT_R32_UINT, m_size.x, m_size.y, 1, 1, 1, BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS, "XeGTAO_WorkingNormals");
                }
            }

            return hadChanges;
        }

        void GTAO::UpdateConstants(const Math::Matrix4& projMatrix, bool usingTAA)
        {
            XeGTAO::GTAOConstants consts;

            XeGTAO::GTAOUpdateConstants(consts, m_size.x, m_size.y, m_settings, &projMatrix[1][1], true, usingTAA);

            {
                Diligent::MapHelper<XeGTAO::GTAOConstants> CBConstants(Graphics::Context::GetInstance().GetContext(), m_constantBuffer, MAP_WRITE, MAP_FLAG_DISCARD);
                *CBConstants = consts;
            }
        }

        void GTAO::Compute(const Components::CameraComponent& camera, bool usingTAA, bool outputBentNormals, Diligent::ITexture* outputAO, Diligent::ITexture* inputDepth, Diligent::ITexture* inputNormals)
        {
            auto outAOdesc = outputAO->GetDesc();
            auto inDepthdesc = inputDepth->GetDesc();

            assert(outAOdesc.GetHeight() == inDepthdesc.GetHeight());
            assert(outAOdesc.GetWidth() == inDepthdesc.GetWidth());
            assert(inDepthdesc.SampleCount == 1); // MSAA no longer supported!

            auto renderContext = Graphics::Context::GetInstance().GetContext();
            m_generateNormals |= inputNormals == nullptr;   // if normals not provided, we must generate them ourselves

            m_outputBentNormals = outputBentNormals;

            if (!m_outputBentNormals) m_debugShowBentNormals = false;

            // when using bent normals, use R32_UINT, otherwise use R8_UINT; these could be anything else as long as the shading side matches
            if (outputBentNormals)
            {
                assert(outAOdesc.Format == TEX_FORMAT_R32_UINT);
            }
            else
            {
                assert(outAOdesc.Format == TEX_FORMAT_R8_UINT);
            }

            UpdateTexturesAndShaders(inDepthdesc.GetWidth(), inDepthdesc.GetHeight());

//#ifdef VA_GTAO_SAMPLE
//            VA_TRACE_CPUGPU_SCOPE_SELECT_BY_DEFAULT(XeGTAO, renderContext);
//#else
//            VA_TRACE_CPUGPU_SCOPE(XeGTAO, renderContext);
//#endif
/*
            if (inputNormals != nullptr)
            {
                auto inNormalsdesc = inputNormals->GetDesc();

                assert(((inNormalsdesc.GetWidth() == m_size.x) || (inNormalsdesc.GetWidth() == m_size.x - 1)) && ((inNormalsdesc.GetHeight() == m_size.y) || (inNormalsdesc.GetHeight() == m_size.y - 1)));
            }
            assert(!m_shadersDirty); if (m_shadersDirty) return; /// vaDrawResultFlags::UnspecifiedError;

            UpdateConstants(camera.GetProjectionMatrix(), usingTAA);
           
          //  vaComputeItem computeItem;
            // UAV barriers not required in current setup because UAV<->SRV barriers are automatically inserted; this however might not
            // hold in future modifications so beware :)
            //computeItem.GlobalUAVBarrierBefore = false;
            //computeItem.GlobalUAVBarrierAfter = false;

            // constants used by all/some passes
            computeItem.ConstantBuffers[0] = m_constantBuffer;
            // SRVs used by all/some passes
            computeItem.BIND_SHADER_RESOURCEViews[5] = m_hilbertLUT;

          ///  // needed only for shader debugging viz
          ///  vaDrawAttributes drawAttributes(camera);

            if (m_generateNormals)
            {
         //       VA_TRACE_CPUGPU_SCOPE(GenerateNormals, renderContext);

              
                computeItem.ComputeShader = m_GenerateNormals;
                DispatchComputeAttribs DispatAttribs((m_size.x + XE_GTAO_NUMTHREADS_X - 1) / XE_GTAO_NUMTHREADS_X, (m_size.y + XE_GTAO_NUMTHREADS_Y - 1) / XE_GTAO_NUMTHREADS_Y, 1);
                // input SRVs
                computeItem.BIND_SHADER_RESOURCEViews[0] = inputDepth;


                Graphics::Context::GetInstance().GetContext()->SetPipelineState(m_GenerateNormals.pPipeline);

                auto RT = m_workingNormals->GetDefaultView(TEXTURE_VIEW_UNORDERED_ACCESS);
                Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, &RT, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                Graphics::Context::GetInstance().GetContext()->CommitShaderResources(m_GenerateNormals.pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                Graphics::Context::GetInstance().GetContext()->DispatchCompute(DispatAttribs);

           ///     renderContext.ExecuteSingleItem(computeItem, vaRenderOutputs::FromUAVs(m_workingNormals), &drawAttributes);
            }

            {
           //     VA_TRACE_CPUGPU_SCOPE(PrefilterDepths, renderContext);

                computeItem.ComputeShader = m_PrefilterDepths16x16;

                // input SRVs
                computeItem.BIND_SHADER_RESOURCEViews[0] = inputDepth;

                // note: in CSPrefilterDepths16x16 each is thread group handles a 16x16 block (with [numthreads(8, 8, 1)] and each logical thread handling a 2x2 block)
                DispatchComputeAttribs DispatAttribs((m_size.x + 16 - 1) / 16, (m_size.y + 16 - 1) / 16, 1);


                Graphics::Context::GetInstance().GetContext()->SetPipelineState(m_PrefilterDepths16x16.pPipeline);

                Graphics::Context::GetInstance().GetContext()->SetRenderTargets(XE_GTAO_DEPTH_MIP_LEVELS, m_workingDepthsMIPViews, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                Graphics::Context::GetInstance().GetContext()->CommitShaderResources(m_PrefilterDepths16x16.pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                Graphics::Context::GetInstance().GetContext()->DispatchCompute(DispatAttribs);
               /// renderContext.ExecuteSingleItem(computeItem, vaRenderOutputs::FromUAVs(m_workingDepthsMIPViews[0], m_workingDepthsMIPViews[1], m_workingDepthsMIPViews[2], m_workingDepthsMIPViews[3], m_workingDepthsMIPViews[4]), &drawAttributes);
            }

            {
             //   VA_TRACE_CPUGPU_SCOPE(MainPass, renderContext);

                GTAOShader shaders[] = { m_GTAOLow, m_GTAOMedium, m_GTAOHigh, m_GTAOUltra };

                // input SRVs
                computeItem.BIND_SHADER_RESOURCEViews[0] = m_workingDepths;
                computeItem.BIND_SHADER_RESOURCEViews[1] = (m_generateNormals) ? (m_workingNormals) : (inputNormals);

                DispatchComputeAttribs DispatAttribs((m_size.x + XE_GTAO_NUMTHREADS_X - 1) / XE_GTAO_NUMTHREADS_X, (m_size.y + XE_GTAO_NUMTHREADS_Y - 1) / XE_GTAO_NUMTHREADS_Y, 1);


                Graphics::Context::GetInstance().GetContext()->SetPipelineState(shaders[m_settings.QualityLevel].pPipeline);

                Diligent::ITextureView* rendertargets[3];
                rendertargets[0] = m_workingNormals->GetDefaultView(TEXTURE_VIEW_UNORDERED_ACCESS);
                rendertargets[1] = m_workingEdges->GetDefaultView(TEXTURE_VIEW_UNORDERED_ACCESS);
                rendertargets[2] = m_debugImage->GetDefaultView(TEXTURE_VIEW_UNORDERED_ACCESS);

                Graphics::Context::GetInstance().GetContext()->SetRenderTargets(3, rendertargets, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                Graphics::Context::GetInstance().GetContext()->CommitShaderResources(shaders[m_settings.QualityLevel].pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                Graphics::Context::GetInstance().GetContext()->DispatchCompute(DispatAttribs);

             //   renderContext.ExecuteSingleItem(computeItem, vaRenderOutputs::FromUAVs(m_workingAOTerm, m_workingEdges, m_debugImage), &drawAttributes);
            }

            {
           //     VA_TRACE_CPUGPU_SCOPE(Denoise, renderContext);
                const int passCount = std::max(1, m_settings.DenoisePasses); // even without denoising we have to run a single last pass to output correct term into the external output texture
                for (int i = 0; i < passCount; i++)
                {
                    const bool lastPass = i == passCount - 1;
                  //  VA_TRACE_CPUGPU_SCOPE(DenoisePass, renderContext);

                    GTAOShader& shader = (lastPass) ? (m_DenoiseLastPass) : (m_DenoisePass);

                    // input SRVs
                    computeItem.BIND_SHADER_RESOURCEViews[0] = m_workingAOTerm;   // see std::swap below
                    computeItem.BIND_SHADER_RESOURCEViews[1] = m_workingEdges;

                    DispatchComputeAttribs DispatAttribs((m_size.x + (XE_GTAO_NUMTHREADS_X * 2) - 1) / (XE_GTAO_NUMTHREADS_X * 2), (m_size.y + XE_GTAO_NUMTHREADS_Y - 1) / XE_GTAO_NUMTHREADS_Y, 1);


                    Graphics::Context::GetInstance().GetContext()->SetPipelineState(shader.pPipeline);

                    Diligent::ITextureView* rendertargets[3];
                    rendertargets[0] = (lastPass) ? (outputAO->GetDefaultView(TEXTURE_VIEW_UNORDERED_ACCESS)) : (m_workingAOTermPong->GetDefaultView(TEXTURE_VIEW_UNORDERED_ACCESS));
                    rendertargets[1] = nullptr;
                    rendertargets[2] = m_debugImage->GetDefaultView(TEXTURE_VIEW_UNORDERED_ACCESS);
                    Graphics::Context::GetInstance().GetContext()->SetRenderTargets(3, rendertargets, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                    Graphics::Context::GetInstance().GetContext()->CommitShaderResources(shader.pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                    Graphics::Context::GetInstance().GetContext()->DispatchCompute(DispatAttribs);

                    std::swap(m_workingAOTerm, m_workingAOTermPong);      // ping becomes pong, pong becomes ping.
                }
            }
            */
            return;
        }
    }
}
