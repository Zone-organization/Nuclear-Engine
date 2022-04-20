#include "Engine\Rendering\DefferedRenderingPipeline.h"
#include <Engine\Graphics\Context.h>

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
        void DefferedRenderingPipeline::Bake(const RenderingPipelineBakingDesc& desc)
        {
            ShadingModelBakingDesc SMDesc = desc.mShadingModelDesc;
            for (auto it : mPairedEffects)
            {
                SMDesc.mRequiredEffects.push_back(it.second);
            }

            GetShadingModel()->Bake(SMDesc);

            mRTWidth = desc.mRTWidth;
            mRTHeight = desc.mRTHeight;

            BakeRenderTargets();
            BakePipeline();
        }
        void DefferedRenderingPipeline::ResizeRenderTarget(Uint32 Width, Uint32 Height)
        {
        }
        void DefferedRenderingPipeline::SetPostProcessingEffect(const Uint32& effectId, bool value)
        {
        }
        void DefferedRenderingPipeline::StartRendering(Systems::RenderSystem* renderer)
        {
        }
        void DefferedRenderingPipeline::SetPipelineState()
        {
        }
        void DefferedRenderingPipeline::BakeRenderTargets()
        {
            Graphics::RenderTargetDesc RTDesc;
            RTDesc.Width = mRTWidth;
            RTDesc.Height = mRTHeight;
            RTDesc.mCreateDepth = false;

            RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;
            PositionRT.Create(RTDesc);
            NormalRT.Create(RTDesc);

            RTDesc.ColorTexFormat = TEX_FORMAT_RGBA8_UNORM;
            AlbedoRT.Create(RTDesc);
        }
        void DefferedRenderingPipeline::BakePipeline()
        {
            std::vector<LayoutElement> Layout;

            Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
            Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV
            Layout.push_back(LayoutElement(2, 0, 3, VT_FLOAT32, false));  //NORMAL
            Layout.push_back(LayoutElement(3, 0, 3, VT_FLOAT32, false));  //Tangents
            Layout.push_back(LayoutElement(4, 0, 4, VT_INT32, false));    //BONE ID
            Layout.push_back(LayoutElement(5, 0, 4, VT_FLOAT32, false));  //WEIGHT
            Graphics::CompoundPipelineDesc PSOCreateInfo;

            for (auto it : mPostProcessingEffects)
            {
                PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch(it.second.GetName()));
            }

            PSOCreateInfo.mVShaderPath = "Assets/NuclearEngine/Shaders/GBuffer.vs.hlsl";
            PSOCreateInfo.mPShaderPath = "Assets/NuclearEngine/Shaders/GBuffer.ps.hlsl";

            PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 3;
            PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = TEX_FORMAT_RGBA16_FLOAT;
            PSOCreateInfo.GraphicsPipeline.RTVFormats[1] = TEX_FORMAT_RGBA16_FLOAT;
            PSOCreateInfo.GraphicsPipeline.RTVFormats[2] = TEX_FORMAT_RGBA8_UNORM;
            //PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
            PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
            PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
            PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
            PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
            PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
            PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

            mPipeline.Create(PSOCreateInfo);

            UpdatePSO(true);
        }
	}
}

            //// Attachment 0 - Position
           //// Attachment 1 - Normal
           //// Attachment 2 - Albedo (Diffuse + Specular)
           //// Attachment 3 - Depth buffer
           //// Attachment 4 - Final color buffer
           //constexpr Uint32 NumAttachments = 5;

           //// Prepare render pass attachment descriptions
           //RenderPassAttachmentDesc Attachments[NumAttachments];
           //Attachments[0].Format = TEX_FORMAT_RGBA16_FLOAT;
           //Attachments[0].InitialState = RESOURCE_STATE_RENDER_TARGET;
           //Attachments[0].FinalState = RESOURCE_STATE_INPUT_ATTACHMENT;
           //Attachments[0].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
           //Attachments[0].StoreOp = ATTACHMENT_STORE_OP_DISCARD; // We will not need the result after the end of the render pass

           //Attachments[1].Format = TEX_FORMAT_RGBA16_FLOAT;
           //Attachments[1].InitialState = RESOURCE_STATE_RENDER_TARGET;
           //Attachments[1].FinalState = RESOURCE_STATE_INPUT_ATTACHMENT;
           //Attachments[1].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
           //Attachments[1].StoreOp = ATTACHMENT_STORE_OP_DISCARD; // We will not need the result after the end of the render pass

           //Attachments[2].Format = TEX_FORMAT_RGBA8_UNORM;
           //Attachments[2].InitialState = RESOURCE_STATE_RENDER_TARGET;
           //Attachments[2].FinalState = RESOURCE_STATE_INPUT_ATTACHMENT;
           //Attachments[2].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
           //Attachments[2].StoreOp = ATTACHMENT_STORE_OP_DISCARD; // We will not need the result after the end of the render pass

           //Attachments[3].Format = info.mDepthBufferFormat;
           //Attachments[3].InitialState = RESOURCE_STATE_DEPTH_WRITE;
           //Attachments[3].FinalState = RESOURCE_STATE_DEPTH_WRITE;
           //Attachments[3].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
           //Attachments[3].StoreOp = ATTACHMENT_STORE_OP_DISCARD; // We will not need the result after the end of the render pass

           //Attachments[4].Format = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
           //Attachments[4].InitialState = RESOURCE_STATE_RENDER_TARGET;
           //Attachments[4].FinalState = RESOURCE_STATE_RENDER_TARGET;
           //Attachments[4].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
           //Attachments[4].StoreOp = ATTACHMENT_STORE_OP_STORE;

           //// Subpass 1 - Render G-buffer
           //// Subpass 2 - Lighting
           //constexpr Uint32 NumSubpasses = 2;

           //// Prepar subpass descriptions
           //SubpassDesc Subpasses[NumSubpasses];

           //// Subpass 0 attachments - 3 render targets
           //AttachmentReference RTAttachmentRefs0[] =
           //{
           //    {0, RESOURCE_STATE_RENDER_TARGET},
           //    {1, RESOURCE_STATE_RENDER_TARGET},
           //    {2, RESOURCE_STATE_RENDER_TARGET}
           //};

           //AttachmentReference DepthAttachmentRef0 = { 3, RESOURCE_STATE_DEPTH_WRITE };

           //////////////////////////////////////////////////////////////////////////////////////////////////////////////
           //// Subpass 1 attachments - 1 render target, depth buffer, 2 input attachments
           //AttachmentReference RTAttachmentRefs1[] =
           //{
           //    {4, RESOURCE_STATE_RENDER_TARGET}
           //};

           //AttachmentReference DepthAttachmentRef1 = { 3, RESOURCE_STATE_DEPTH_WRITE };

           //AttachmentReference InputAttachmentRefs1[] =
           //{
           //    {0, RESOURCE_STATE_INPUT_ATTACHMENT},
           //    {1, RESOURCE_STATE_INPUT_ATTACHMENT},
           //    {2, RESOURCE_STATE_INPUT_ATTACHMENT}
           //};

           //Subpasses[0].RenderTargetAttachmentCount = _countof(RTAttachmentRefs0);
           //Subpasses[0].pRenderTargetAttachments = RTAttachmentRefs0;
           //Subpasses[0].pDepthStencilAttachment = &DepthAttachmentRef0;

           //Subpasses[1].RenderTargetAttachmentCount = _countof(RTAttachmentRefs1);
           //Subpasses[1].pRenderTargetAttachments = RTAttachmentRefs1;
           //Subpasses[1].pDepthStencilAttachment = &DepthAttachmentRef1;
           //Subpasses[1].InputAttachmentCount = _countof(InputAttachmentRefs1);
           //Subpasses[1].pInputAttachments = InputAttachmentRefs1;

           //// We need to define dependency between subpasses 0 and 1 to ensure that
           //// all writes are complete before we use the attachments for input in subpass 1.
           //SubpassDependencyDesc Dependencies[1];
           //Dependencies[0].SrcSubpass = 0;
           //Dependencies[0].DstSubpass = 1;
           //Dependencies[0].SrcStageMask = PIPELINE_STAGE_FLAG_RENDER_TARGET;
           //Dependencies[0].DstStageMask = PIPELINE_STAGE_FLAG_PIXEL_SHADER;
           //Dependencies[0].SrcAccessMask = ACCESS_FLAG_RENDER_TARGET_WRITE;
           //Dependencies[0].DstAccessMask = ACCESS_FLAG_SHADER_READ;

           //RenderPassDesc RPDesc;
           //RPDesc.Name = "Deffered Rendering Pipeline RenderPass";
           //RPDesc.AttachmentCount = _countof(Attachments);
           //RPDesc.pAttachments = Attachments;
           //RPDesc.SubpassCount = _countof(Subpasses);
           //RPDesc.pSubpasses = Subpasses;
           //RPDesc.DependencyCount = _countof(Dependencies);
           //RPDesc.pDependencies = Dependencies;

           //Graphics::Context::GetDevice()->CreateRenderPass(RPDesc, &mRenderPass);