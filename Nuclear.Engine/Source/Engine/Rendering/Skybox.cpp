#include "Engine/Rendering/Skybox.h"
#include <Engine/Graphics/Context.h>
#include <Engine/Managers/AssetManager.h>
#include <Engine/Components/CameraComponent.h>
#include "Engine/Graphics/GraphicsEngine.h"
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Core/FileSystem.h>

namespace Nuclear
{
	namespace Rendering
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

		void Skybox::Initialize(IBuffer* CameraConstantBuffer, const std::array<Assets::Image*, 6> & data)
		{
			TextureDesc TexDesc;
			TexDesc.Name = "SkyBox_TextureCube";
			TexDesc.Type = RESOURCE_DIM_TEX_CUBE;
			TexDesc.Usage = USAGE_DEFAULT;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE;
			TexDesc.Width = data.at(0)->mData.mWidth;
			TexDesc.Height = data.at(0)->mData.mHeight;
			TexDesc.Format = TEX_FORMAT_RGBA8_UNORM;
			TexDesc.ArraySize = 6;
			TexDesc.MipLevels = 1;

			RefCntAutoPtr<ITexture> TexCube;
			TextureData Data;


			TextureSubResData subData[6];
			for (unsigned int i = 0; i < data.size(); i++)
			{
				subData[i].pData = data[i]->mData.mData;
				subData[i].Stride = data[i]->mData.mWidth * 4;
			}

			Data.NumSubresources = 6;
			Data.pSubResources = &subData[0];
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(TexDesc, &Data, &TexCube);
			if (TexCube != nullptr)
			{
				mTextureSRV = TexCube->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

				InitializePipeline(CameraConstantBuffer);
				InitializeCube();
			}
		}

		void Skybox::Initialize(IBuffer* CameraConstantBuffer, Assets::Image* tex)
		{
			mTextureSRV = tex->mTextureView;

			InitializePipeline(CameraConstantBuffer);
			InitializeCube();
		}
	

		void Skybox::Render()
		{
			if (Valid)
			{
				Graphics::Context::GetInstance().GetContext()->SetPipelineState(mPipeline);
				Graphics::Context::GetInstance().GetContext()->CommitShaderResources(mSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				RenderCube();
			}
		}

		void Skybox::InitializePipeline(IBuffer* CameraConstantBuffer)
		{
			GraphicsPipelineStateCreateInfo PSOCreateInfo;
			PSOCreateInfo.PSODesc.Name = "SkyBox_PSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthFunc = COMPARISON_FUNC_LESS_EQUAL;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthWriteEnable = false;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;
			Graphics::GraphicsEngine::GetInstance().GetShaderManager().CreateShader(Core::FileSystem::LoadFileToString("@NuclearAssets@/Shaders/Background.vs.hlsl"), VSShader.RawDblPtr(), SHADER_TYPE_VERTEX);
			Graphics::GraphicsEngine::GetInstance().GetShaderManager().CreateShader(Core::FileSystem::LoadFileToString("@NuclearAssets@/Shaders/Background.Ps.hlsl"), PSShader.RawDblPtr(), SHADER_TYPE_PIXEL);


			LayoutElement LayoutElems[] =
			{
				// Attribute 0 - vertex position
				LayoutElement{0, 0, 3, VT_FLOAT32, False,0},
			};

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);
			PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

			ShaderResourceVariableDesc Vars[] =
			{
				{SHADER_TYPE_PIXEL, "NE_Skybox", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
			};
			PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
			PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

			// Define static sampler for g_Texture. Static samplers should be used whenever possible
			SamplerDesc SamLinearClampDesc(FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
				TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP);

			SamLinearClampDesc.ComparisonFunc = COMPARISON_FUNC_ALWAYS;
			ImmutableSamplerDesc StaticSamplers[] =
			{
				{SHADER_TYPE_PIXEL, "NE_Skybox", SamLinearClampDesc}
			};
			PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = StaticSamplers;
			PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(StaticSamplers);
			Graphics::Context::GetInstance().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &mPipeline);

			mPipeline->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_Camera")->Set(CameraConstantBuffer);


			mPipeline->CreateShaderResourceBinding(&mSRB, true);
			mSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(mTextureSRV);
			Valid = true;
		}

		void Skybox::InitializeCube()
		{
			BufferDesc VertBuffDesc;
			VertBuffDesc.Usage = USAGE_IMMUTABLE;
			VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
			VertBuffDesc.Size = sizeof(skyboxVertices);

			BufferData VBData;
			VBData.pData = skyboxVertices;
			VBData.DataSize = sizeof(skyboxVertices);
			Graphics::Context::GetInstance().GetDevice()->CreateBuffer(VertBuffDesc, &VBData, &mVBuffer);
		}

		void Skybox::RenderCube()
		{
			Graphics::Context::GetInstance().GetContext()->SetVertexBuffers(0, 1, &mVBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

			DrawAttribs DrawAttrs;
			DrawAttrs.NumVertices = 36;
			Graphics::Context::GetInstance().GetContext()->Draw(DrawAttrs);
		}

	}
}