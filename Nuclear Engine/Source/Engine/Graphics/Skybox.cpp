#include "Engine/Graphics/Skybox.h"
#include <Engine/Graphics/Context.h>
#include <Engine/Managers/AssetManager.h>
#include <Engine/Components/CameraComponent.h>
#include "Engine/Graphics/GraphicsEngine.h"
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Core/FileSystem.h>

namespace NuclearEngine
{
	namespace Graphics
	{

		float skyboxVertices[] = {
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f, 
			-1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f, 

			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f, 
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f, 

			1.0f,  1.0f,  1.0f, 
			1.0f,  1.0f, -1.0f, 
			1.0f, -1.0f, -1.0f, 
			1.0f, -1.0f, -1.0f, 
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f, 

			-1.0f, -1.0f, -1.0f, 
			1.0f, -1.0f, -1.0f, 
			1.0f, -1.0f,  1.0f, 
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,  
			-1.0f, -1.0f, -1.0f, 

			-1.0f,  1.0f, -1.0f, 
			1.0f,  1.0f, -1.0f, 
			1.0f,  1.0f,  1.0f, 
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f, 
			-1.0f,  1.0f, -1.0f 
		};

		Skybox::Skybox()
		{
		}
		Skybox::~Skybox()
		{
		
		}

		void Skybox::Initialize(IBuffer* CameraConstantBuffer, const std::array<Assets::Image, 6> & data)
		{
			TextureDesc TexDesc;
			TexDesc.Name = "SkyBox_TextureCube";
			TexDesc.Type = RESOURCE_DIM_TEX_CUBE;
			TexDesc.Usage = USAGE_DEFAULT;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE;
			TexDesc.Width = data.at(0).mWidth;
			TexDesc.Height = data.at(0).mHeight;
			TexDesc.Format = TEX_FORMAT_RGBA8_UNORM;
			TexDesc.ArraySize = 6;
			TexDesc.MipLevels = 1;

			RefCntAutoPtr<ITexture> TexCube;
			TextureData Data;


			TextureSubResData subData[6];
			for (unsigned int i = 0; i < data.size(); i++)
			{
				subData[i].pData = data[i].mData;
				subData[i].Stride = data[i].mWidth * 4;
			}

			Data.NumSubresources = 6;
			Data.pSubResources = &subData[0];
			Graphics::Context::GetDevice()->CreateTexture(TexDesc, &Data, &TexCube);
			mTextureSRV = TexCube->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

			PipelineStateDesc PSODesc;
			PSODesc.Name = "SkyBox_PSO";
			PSODesc.IsComputePipeline = false;
			PSODesc.GraphicsPipeline.NumRenderTargets = 1;
			PSODesc.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSODesc.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSODesc.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSODesc.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSODesc.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSODesc.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSODesc.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
			PSODesc.GraphicsPipeline.DepthStencilDesc.DepthFunc = COMPARISON_FUNC_LESS_EQUAL;
			PSODesc.GraphicsPipeline.DepthStencilDesc.DepthWriteEnable = false;
			PSODesc.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;
			VSShader = Graphics::GraphicsEngine::GetShaderManager()->CreateShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl"), SHADER_TYPE_VERTEX);
			PSShader = Graphics::GraphicsEngine::GetShaderManager()->CreateShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.Ps.hlsl"), SHADER_TYPE_PIXEL);

			
			LayoutElement LayoutElems[] =
			{
				// Attribute 0 - vertex position
				LayoutElement{0, 0, 3, VT_FLOAT32, False,0},
			};

			PSODesc.GraphicsPipeline.pVS = VSShader;
			PSODesc.GraphicsPipeline.pPS = PSShader;
			PSODesc.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
			PSODesc.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);
			PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

			ShaderResourceVariableDesc Vars[] =
			{
				{SHADER_TYPE_PIXEL, "NE_Skybox", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
			};
			PSODesc.ResourceLayout.Variables = Vars;
			PSODesc.ResourceLayout.NumVariables = _countof(Vars);
			
			// Define static sampler for g_Texture. Static samplers should be used whenever possible
			SamplerDesc SamLinearClampDesc(FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
				TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP);

			SamLinearClampDesc.ComparisonFunc = COMPARISON_FUNC_ALWAYS;
			StaticSamplerDesc StaticSamplers[] =
			{
				{SHADER_TYPE_PIXEL, "NE_Skybox", SamLinearClampDesc}
			};
			PSODesc.ResourceLayout.StaticSamplers = StaticSamplers;
			PSODesc.ResourceLayout.NumStaticSamplers = _countof(StaticSamplers);

			Graphics::Context::GetDevice()->CreatePipelineState(PSODesc, &mPipeline);
			mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(CameraConstantBuffer);


			mPipeline->CreateShaderResourceBinding(&mSRB, true);
			mSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(mTextureSRV);

			BufferDesc VertBuffDesc;
			VertBuffDesc.Usage = USAGE_STATIC;
			VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
			VertBuffDesc.uiSizeInBytes = sizeof(skyboxVertices);

			BufferData VBData;
			VBData.pData = skyboxVertices;
			VBData.DataSize = sizeof(skyboxVertices);
			Graphics::Context::GetDevice()->CreateBuffer(VertBuffDesc, &VBData, &mVBuffer);
		}
	

		void Skybox::Render()
		{
			Graphics::Context::GetContext()->SetPipelineState(mPipeline);
			Graphics::Context::GetContext()->CommitShaderResources(mSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetContext()->SetVertexBuffers(0, 1,&mVBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

			DrawAttribs DrawAttrs;
			DrawAttrs.NumVertices = 36;
			Graphics::Context::GetContext()->Draw(DrawAttrs);
		}

	}
}