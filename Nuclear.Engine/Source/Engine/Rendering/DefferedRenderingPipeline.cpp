#include "Engine\Rendering\DefferedRenderingPipeline.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Components/MeshComponent.h>
#include <Engine\Components/EntityInfoComponent.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Components\AnimatorComponent.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Core\FileSystem.h>
#include <Engine/Graphics/ImGui.h>

namespace Nuclear
{
	namespace Rendering
	{
		DefferedRenderingPipeline::DefferedRenderingPipeline(const std::string& name)
			: RenderingPipeline(name)
		{
		}
		void DefferedRenderingPipeline::Initialize(const DefferedRenderingPipelineInitInfo& info)
		{
            SetShadingModelAndCamera(info.shadingModel, info.camera);
		}

        void DefferedRenderingPipeline::ResizeRenderTargets(Uint32 Width, Uint32 Height)
        {

            RenderingPipeline::ResizeRenderTargets(Width, Height);
        }

        void DefferedRenderingPipeline::StartRendering(Systems::RenderSystem* renderer)
        {
            std::vector<ITextureView*> RTargets;
            RTargets.push_back(mGBuffer.mPositonBuffer->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET));
            RTargets.push_back(mGBuffer.mNormalBuffer->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET));
            RTargets.push_back(mGBuffer.mAlbedoBuffer->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET));


            Graphics::Context::GetContext()->SetRenderTargets(RTargets.size(), RTargets.data(), mGBuffer.pDepthBuffer->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            for (auto& i : RTargets)
            {
                Graphics::Context::GetContext()->ClearRenderTarget(i, (float*)&GetCamera()->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            }

              Graphics::Context::GetContext()->ClearDepthStencil(mGBuffer.pDepthBuffer->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            //Render To Gbuffer
            Graphics::Context::GetContext()->SetPipelineState(GetShadingModel()->GetGBufferPipeline());
            RenderMeshes(renderer);

            if (GetCamera()->RenderSkybox == true && GetCamera()->mSkybox != nullptr)
            {
                GetCamera()->mSkybox->Render();
            }

            //Apply Lighting
            //Graphics::Context::GetContext()->SetPipelineState(GetShadingModel()->GetShadersPipeline());
            SetPostFXPipelineForRendering();
           // Graphics::Context::GetContext()->SetRenderTargets(1, SceneRT.mColorRTV.RawDblPtr(), mGBuffer.pDepthBuffer->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
           // Graphics::Context::GetContext()->ClearRenderTarget(SceneRT.mColorRTV.RawPtr(), (float*)&GetCamera()->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            //Graphics::Context::GetContext()->ClearDepthStencil(GetSceneRT()->mDepthDSV.RawPtr(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            GetShadingModel()->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(mGBuffer.mPositonBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
            GetShadingModel()->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 1)->Set(mGBuffer.mNormalBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
            GetShadingModel()->GetShadersPipelineSRB()->GetVariableByIndex(SHADER_TYPE_PIXEL, 2)->Set(mGBuffer.mAlbedoBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
            Graphics::Context::GetContext()->CommitShaderResources(GetShadingModel()->GetShadersPipelineSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            Assets::DefaultMeshes::RenderScreenQuad();

            ApplyPostProcessingEffects();

            ImGui::Begin("GBUFFER");
            ImGui::Image(mGBuffer.mPositonBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE), { 256.f,256.f });
            ImGui::SameLine();
            ImGui::Image(mGBuffer.mNormalBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE), { 256.f,256.f });
            ImGui::Image(mGBuffer.mAlbedoBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE), { 256.f,256.f });
            ImGui::End();

        }
        void DefferedRenderingPipeline::BakeRenderTargets()
        {
            //Create Color Texture
            TextureDesc TexDesc;
            TexDesc.Type = RESOURCE_DIM_TEX_2D;
            TexDesc.Width = mRTWidth;
            TexDesc.Height = mRTHeight;

            TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
            TexDesc.MipLevels = 1;

            TexDesc.Format = TEX_FORMAT_RGBA16_FLOAT;
            Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &mGBuffer.mPositonBuffer);
            Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &mGBuffer.mNormalBuffer);

            TexDesc.Format = TEX_FORMAT_RGBA8_UNORM;
            Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &mGBuffer.mAlbedoBuffer);

            //Create Depth Texture
            TexDesc.Format = TEX_FORMAT_D32_FLOAT;
            TexDesc.ClearValue.Format = TexDesc.Format;
            TexDesc.ClearValue.DepthStencil.Depth = 1;
            TexDesc.ClearValue.DepthStencil.Stencil = 0;
            TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;
            Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &mGBuffer.pDepthBuffer);

            RenderingPipeline::BakeRenderTargets();
        }

        void DefferedRenderingPipeline::RenderMeshes(Systems::RenderSystem* renderer)
        {
            auto view = renderer->mScene->GetRegistry().view<Components::MeshComponent>();
            for (auto entity : view)
            {
                auto& MeshObject = view.get<Components::MeshComponent>(entity);
                if (MeshObject.mRender)
                {
                    auto& EntityInfo = renderer->mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
                    EntityInfo.mTransform.Update();
                    GetCamera()->SetModelMatrix(EntityInfo.mTransform.GetWorldMatrix());
                    renderer->GetCameraSubSystem().UpdateBuffer();

                    auto AnimatorComponent = renderer->mScene->GetRegistry().try_get<Components::AnimatorComponent>(entity);

                    if (AnimatorComponent != nullptr)
                    {
                        std::vector<Math::Matrix4> ok;
                        ok.reserve(100);

                        auto transforms = AnimatorComponent->mAnimator->GetFinalBoneMatrices();
                        for (int i = 0; i < transforms.size(); ++i)
                        {
                            ok.push_back(transforms[i]);
                        }

                        PVoid data;
                        Graphics::Context::GetContext()->MapBuffer(renderer->GetAnimationCB(), MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
                        data = memcpy(data, ok.data(), ok.size() * sizeof(Math::Matrix4));
                        Graphics::Context::GetContext()->UnmapBuffer(renderer->GetAnimationCB(), MAP_WRITE);

                    }
                    else {
                        Math::Matrix4 empty(0.0f);
                        PVoid data;
                        Graphics::Context::GetContext()->MapBuffer(renderer->GetAnimationCB(), MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
                        data = memcpy(data, &empty, sizeof(Math::Matrix4));
                        Graphics::Context::GetContext()->UnmapBuffer(renderer->GetAnimationCB(), MAP_WRITE);
                    }

                    InstantRender(MeshObject.mMesh, MeshObject.mMaterial);
                }
            }
        }
	}
}