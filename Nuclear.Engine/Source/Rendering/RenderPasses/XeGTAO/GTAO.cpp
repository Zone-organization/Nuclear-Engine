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

#define VA_IMGUI_INTEGRATION_ENABLED
namespace Nuclear
{
    namespace Rendering
    {

        GTAO::GTAO() :
            vaRenderingModule(params),
            vaUIPanel("XeGTAO", 10, !VA_MINIMAL_UI_BOOL, vaUIPanel::DockLocation::DockedLeftBottom),
            m_constantBuffer(vaConstantBuffer::Create<XeGTAO::GTAOConstants>(params.RenderDevice, "GTAOConstants"))
        {
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
                m_hilbertLUT = vaTexture::Create2D(GetRenderDevice(), vaResourceFormat::R16_UINT, 64, 64, 1, 1, 1, vaResourceBindSupportFlags::ShaderResource, vaResourceAccessFlags::Default, vaResourceFormat::Automatic, vaResourceFormat::Automatic, vaResourceFormat::Automatic, vaResourceFormat::Automatic, vaTextureFlags::None, vaTextureContentsType::GenericLinear,
                    data, 64 * 2);
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

            if (m_shadersDirty)
            {
                m_shadersDirty = false;

                // to allow parallel background compilation but still ensure they're all compiled after this function
                std::vector<shared_ptr<vaShader>> allShaders;
                string shaderFileToUse = "GTAO.hlsl";

                allShaders.push_back(m_PrefilterDepths16x16 = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSPrefilterDepths16x16", m_staticShaderMacros, false));
                allShaders.push_back(m_GTAOLow = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSGTAOLow", m_staticShaderMacros, false));
                allShaders.push_back(m_GTAOMedium = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSGTAOMedium", m_staticShaderMacros, false));
                allShaders.push_back(m_GTAOHigh = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSGTAOHigh", m_staticShaderMacros, false));
                allShaders.push_back(m_GTAOUltra = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSGTAOUltra", m_staticShaderMacros, false));
                allShaders.push_back(m_DenoisePass = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSDenoisePass", m_staticShaderMacros, false));
                allShaders.push_back(m_DenoiseLastPass = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSDenoiseLastPass", m_staticShaderMacros, false));
                allShaders.push_back(m_GenerateNormals = vaComputeShader::CreateFromFile(GetRenderDevice(), shaderFileToUse, "CSGenerateNormals", m_staticShaderMacros, false));

                // wait until shaders are compiled! this allows for parallel compilation but ensures all are compiled after this point
                for (auto sh : allShaders) sh->WaitFinishIfBackgroundCreateActive();

                hadChanges = true;
            }

            bool needsUpdate = false;

            needsUpdate |= (m_size.x != width) || (m_size.y != height);
            needsUpdate |= (m_workingNormals == nullptr) && m_generateNormals;

            vaResourceFormat requiredDepthFormat = (m_use32bitDepth) ? (vaResourceFormat::R32_FLOAT) : (vaResourceFormat::R16_FLOAT);
            needsUpdate |= m_workingDepths == nullptr || m_workingDepths->GetResourceFormat() != requiredDepthFormat;
            vaResourceFormat requiredAOTermFormat = (m_outputBentNormals) ? (vaResourceFormat::R32_UINT) : (vaResourceFormat::R8_UINT);
            needsUpdate |= m_workingAOTerm == nullptr || m_workingAOTerm->GetResourceFormat() != requiredAOTermFormat;

            m_size.x = width;
            m_size.y = height;

            if (needsUpdate)
            {
                hadChanges = true;

                m_debugImage = vaTexture::Create2D(GetRenderDevice(), vaResourceFormat::R11G11B10_FLOAT, m_size.x, m_size.y, 1, 1, 1, vaResourceBindSupportFlags::ShaderResource | vaResourceBindSupportFlags::UnorderedAccess);
                m_workingDepths = vaTexture::Create2D(GetRenderDevice(), requiredDepthFormat, m_size.x, m_size.y, XE_GTAO_DEPTH_MIP_LEVELS, 1, 1, vaResourceBindSupportFlags::ShaderResource | vaResourceBindSupportFlags::UnorderedAccess);
                for (int mip = 0; mip < XE_GTAO_DEPTH_MIP_LEVELS; mip++)
                    m_workingDepthsMIPViews[mip] = vaTexture::CreateView(m_workingDepths, m_workingDepths->GetBindSupportFlags(), vaResourceFormat::Automatic, vaResourceFormat::Automatic, vaResourceFormat::Automatic, vaResourceFormat::Automatic, vaTextureFlags::None, mip, 1);
                m_workingEdges = vaTexture::Create2D(GetRenderDevice(), vaResourceFormat::R8_UNORM, m_size.x, m_size.y, 1, 1, 1, vaResourceBindSupportFlags::ShaderResource | vaResourceBindSupportFlags::UnorderedAccess);
                m_workingAOTerm = vaTexture::Create2D(GetRenderDevice(), requiredAOTermFormat, m_size.x, m_size.y, 1, 1, 1, vaResourceBindSupportFlags::ShaderResource | vaResourceBindSupportFlags::UnorderedAccess);
                m_workingAOTermPong = vaTexture::Create2D(GetRenderDevice(), requiredAOTermFormat, m_size.x, m_size.y, 1, 1, 1, vaResourceBindSupportFlags::ShaderResource | vaResourceBindSupportFlags::UnorderedAccess);

                if (m_generateNormals)
                {
                    m_workingNormals = vaTexture::Create2D(GetRenderDevice(), vaResourceFormat::R32_UINT, m_size.x, m_size.y, 1, 1, 1, vaResourceBindSupportFlags::ShaderResource | vaResourceBindSupportFlags::UnorderedAccess);
                    m_workingNormals->SetName("XeGTAO_WorkingNormals");
                }

                m_debugImage->SetName("XeGTAO_DebugImage");
                m_workingDepths->SetName("XeGTAO_WorkingDepths");
                m_workingEdges->SetName("XeGTA_WorkingEdges");
                m_workingAOTerm->SetName("XeGTAO_WorkingAOTerm1");
                m_workingAOTermPong->SetName("XeGTAO_WorkingAOTerm2");
            }

            return hadChanges;
        }

        void GTAO::UpdateConstants(vaRenderDeviceContext& renderContext, const vaMatrix4x4& projMatrix, bool usingTAA)
        {
            XeGTAO::GTAOConstants consts;

            XeGTAO::GTAOUpdateConstants(consts, m_size.x, m_size.y, m_settings, &projMatrix._11, true, (usingTAA) ? (GetRenderDevice().GetCurrentFrameIndex() % 256) : (0));

            m_constantBuffer->Upload(renderContext, consts);
        }

        void GTAO::Compute(const Components::CameraComponent& cameraBase, bool usingTAA, bool outputBentNormals, const Diligent::RefCntAutoPtr<Diligent::ITextureView>& outputAO, const Diligent::RefCntAutoPtr<Diligent::ITextureView>& inputDepth, const Diligent::RefCntAutoPtr<Diligent::ITextureView>& inputNormals)
        {
            assert(outputAO->GetSize() == inputDepth->GetSize());
            assert(inputDepth->GetSampleCount() == 1); // MSAA no longer supported!

            auto renderContext = Graphics::Context::GetInstance().GetContext();
            m_generateNormals |= inputNormals == nullptr;   // if normals not provided, we must generate them ourselves

            m_outputBentNormals = outputBentNormals;

            if (!m_outputBentNormals) m_debugShowBentNormals = false;

            // when using bent normals, use R32_UINT, otherwise use R8_UINT; these could be anything else as long as the shading side matches
            if (outputBentNormals)
            {
                assert(outputAO->GetUAVFormat() == vaResourceFormat::R32_UINT);
            }
            else
            {
                assert(outputAO->GetUAVFormat() == vaResourceFormat::R8_UINT);
            }

            UpdateTexturesAndShaders(inputDepth->GetSizeX(), inputDepth->GetSizeY());

#ifdef VA_GTAO_SAMPLE
            VA_TRACE_CPUGPU_SCOPE_SELECT_BY_DEFAULT(XeGTAO, renderContext);
#else
            VA_TRACE_CPUGPU_SCOPE(XeGTAO, renderContext);
#endif

            if (inputNormals != nullptr)
            {
                assert(((inputNormals->GetSizeX() == m_size.x) || (inputNormals->GetSizeX() == m_size.x - 1)) && ((inputNormals->GetSizeY() == m_size.y) || (inputNormals->GetSizeY() == m_size.y - 1)));
            }
            assert(!m_shadersDirty); if (m_shadersDirty) return vaDrawResultFlags::UnspecifiedError;

            UpdateConstants(renderContext, camera.GetProjMatrix(), usingTAA);

            vaComputeItem computeItem;
            // UAV barriers not required in current setup because UAV<->SRV barriers are automatically inserted; this however might not
            // hold in future modifications so beware :)
            computeItem.GlobalUAVBarrierBefore = false;
            computeItem.GlobalUAVBarrierAfter = false;

            // constants used by all/some passes
            computeItem.ConstantBuffers[0] = m_constantBuffer;
            // SRVs used by all/some passes
            computeItem.ShaderResourceViews[5] = m_hilbertLUT;

            // needed only for shader debugging viz
            vaDrawAttributes drawAttributes(camera);

            if (m_generateNormals)
            {
                VA_TRACE_CPUGPU_SCOPE(GenerateNormals, renderContext);

                computeItem.ComputeShader = m_GenerateNormals;

                // input SRVs
                computeItem.ShaderResourceViews[0] = inputDepth;

                computeItem.SetDispatch((m_size.x + XE_GTAO_NUMTHREADS_X - 1) / XE_GTAO_NUMTHREADS_X, (m_size.y + XE_GTAO_NUMTHREADS_Y - 1) / XE_GTAO_NUMTHREADS_Y, 1);

                renderContext.ExecuteSingleItem(computeItem, vaRenderOutputs::FromUAVs(m_workingNormals), &drawAttributes);
            }

            {
                VA_TRACE_CPUGPU_SCOPE(PrefilterDepths, renderContext);

                computeItem.ComputeShader = m_PrefilterDepths16x16;

                // input SRVs
                computeItem.ShaderResourceViews[0] = inputDepth;

                // note: in CSPrefilterDepths16x16 each is thread group handles a 16x16 block (with [numthreads(8, 8, 1)] and each logical thread handling a 2x2 block)
                computeItem.SetDispatch((m_size.x + 16 - 1) / 16, (m_size.y + 16 - 1) / 16, 1);

                renderContext.ExecuteSingleItem(computeItem, vaRenderOutputs::FromUAVs(m_workingDepthsMIPViews[0], m_workingDepthsMIPViews[1], m_workingDepthsMIPViews[2], m_workingDepthsMIPViews[3], m_workingDepthsMIPViews[4]), &drawAttributes);
            }

            {
                VA_TRACE_CPUGPU_SCOPE(MainPass, renderContext);

                shared_ptr<vaComputeShader> shaders[] = { m_CSGTAOLow, m_CSGTAOMedium, m_CSGTAOHigh, m_CSGTAOUltra };
                computeItem.ComputeShader = shaders[m_settings.QualityLevel];

                // input SRVs
                computeItem.ShaderResourceViews[0] = m_workingDepths;
                computeItem.ShaderResourceViews[1] = (m_generateNormals) ? (m_workingNormals) : (inputNormals);

                computeItem.SetDispatch((m_size.x + XE_GTAO_NUMTHREADS_X - 1) / XE_GTAO_NUMTHREADS_X, (m_size.y + XE_GTAO_NUMTHREADS_Y - 1) / XE_GTAO_NUMTHREADS_Y, 1);

                renderContext.ExecuteSingleItem(computeItem, vaRenderOutputs::FromUAVs(m_workingAOTerm, m_workingEdges, m_debugImage), &drawAttributes);
            }

            {
                VA_TRACE_CPUGPU_SCOPE(Denoise, renderContext);
                const int passCount = std::max(1, m_settings.DenoisePasses); // even without denoising we have to run a single last pass to output correct term into the external output texture
                for (int i = 0; i < passCount; i++)
                {
                    const bool lastPass = i == passCount - 1;
                    VA_TRACE_CPUGPU_SCOPE(DenoisePass, renderContext);

                    computeItem.ComputeShader = (lastPass) ? (m_DenoiseLastPass) : (m_DenoisePass);

                    // input SRVs
                    computeItem.ShaderResourceViews[0] = m_workingAOTerm;   // see std::swap below
                    computeItem.ShaderResourceViews[1] = m_workingEdges;

                    computeItem.SetDispatch((m_size.x + (XE_GTAO_NUMTHREADS_X * 2) - 1) / (XE_GTAO_NUMTHREADS_X * 2), (m_size.y + XE_GTAO_NUMTHREADS_Y - 1) / XE_GTAO_NUMTHREADS_Y, 1);

                    renderContext.ExecuteSingleItem(computeItem, vaRenderOutputs::FromUAVs((lastPass) ? (outputAO) : (m_workingAOTermPong), nullptr, m_debugImage), &drawAttributes);
                    std::swap(m_workingAOTerm, m_workingAOTermPong);      // ping becomes pong, pong becomes ping.
                }
            }

            return;
        }
    }
}
