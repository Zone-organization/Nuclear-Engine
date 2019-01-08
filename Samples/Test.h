#pragma once
#include "Common.h"
#include <Diligent\Graphics\GraphicsEngine\interface\MapHelper.h>
#include <Diligent/Graphics/GraphicsTools/include/BasicShaderSourceStreamFactory.h>
#include <Diligent/Graphics/GraphicsTools/include/GraphicsUtilities.h>
#include <Diligent\Common\interface\BasicMath.h>

#pragma comment(lib,"Common.lib")
#pragma comment(lib,"GraphicsTools.lib")
#pragma comment(lib,"Win32Platform.lib")
#pragma comment(lib,"BasicPlatform.lib")

class Test : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	Core::Input Input;

	RefCntAutoPtr<IShaderResourceBinding> mSRB;

	//Asset Manager (Loader)
	Managers::AssetManager AssetLoader;

	Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> m_pSRB;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> m_CubeVertexBuffer;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> m_CubeIndexBuffer;
	Diligent::RefCntAutoPtr<Diligent::IBuffer> m_VSConstants;
	float4x4 m_WorldViewProjMatrix;


	void Load()
	{
		{
			// Pipeline state object encompasses configuration of all GPU stages

			PipelineStateDesc PSODesc;
			// Pipeline state name is used by the engine to report issues
			// It is always a good idea to give objects descriptive names
			PSODesc.Name = "Cube PSO";

			// This is a graphics pipeline
			PSODesc.IsComputePipeline = false;

			// This tutorial will render to a single render target
			PSODesc.GraphicsPipeline.NumRenderTargets = 1;
			// Set render target format which is the format of the swap chain's color buffer
			PSODesc.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			// Set depth buffer format which is the format of the swap chain's back buffer
			PSODesc.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			// Primitive topology defines what kind of primitives will be rendered by this pipeline state
			PSODesc.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			// Cull back faces
			PSODesc.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			// Enable depth testing
			PSODesc.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

			ShaderCreationAttribs CreationAttribs;
			// Tell the system that the shader source code is in HLSL.
			// For OpenGL, the engine will convert this into GLSL behind the scene
			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

			// We will be using combined texture samplers
			CreationAttribs.UseCombinedTextureSamplers = true;

			// In this tutorial, we will load shaders from file. To be able to do that,
			// we need to create a shader source stream factory
			BasicShaderSourceStreamFactory BasicSSSFactory;
			CreationAttribs.pShaderSourceStreamFactory = &BasicSSSFactory;
			// Define variable type that will be used by default
			CreationAttribs.Desc.DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
			// Create vertex shader
			RefCntAutoPtr<IShader> pVS;
			{
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "Cube VS";
				CreationAttribs.FilePath = "cube.vsh";
				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pVS);
				// Create dynamic uniform buffer that will store our transformation matrix
				// Dynamic buffers can be frequently updated by the CPU
				BufferDesc CBDesc;
				CBDesc.Name = "VS constants CB";
				CBDesc.uiSizeInBytes = sizeof(Math::Matrix4);
				CBDesc.Usage = USAGE_DYNAMIC;
				CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
				CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
				Graphics::Context::GetDevice()->CreateBuffer(CBDesc, BufferData(), &m_VSConstants);

				// Since we did not explcitly specify the type for Constants, default type
				// (SHADER_VARIABLE_TYPE_STATIC) will be used. Static variables never change and are bound directly
				// through the shader (http://diligentgraphics.com/2016/03/23/resource-binding-model-in-diligent-engine-2-0/)
				pVS->GetShaderVariable("Constants")->Set(m_VSConstants);
			}

			// Create pixel shader
			RefCntAutoPtr<IShader> pPS;
			{
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "Cube PS";
				CreationAttribs.FilePath = "cube.psh";
				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pPS);
			}

			// Define vertex shader input layout
			LayoutElement LayoutElems[] =
			{
				// Attribute 0 - vertex position
				LayoutElement(0, 0, 3, VT_FLOAT32, False),
				// Attribute 1 - vertex color
				LayoutElement(1, 0, 4, VT_FLOAT32, False)
			};
			PSODesc.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
			PSODesc.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

			PSODesc.GraphicsPipeline.pVS = pVS;
			PSODesc.GraphicsPipeline.pPS = pPS;

			Graphics::Context::GetDevice()->CreatePipelineState(PSODesc, &m_pPSO);

			// Create shader resource binding object and bind all static resources in it
			m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
		}

		{
			// Layout of this structure matches the one we defined in pipeline state
			struct Vertex
			{
				Math::Vector3 pos;
				Math::Vector4 color;
			};

			// Cube vertices

			//      (-1,+1,+1)________________(+1,+1,+1) 
			//               /|              /|
			//              / |             / |
			//             /  |            /  |
			//            /   |           /   |
			//(-1,-1,+1) /____|__________/(+1,-1,+1)
			//           |    |__________|____| 
			//           |   /(-1,+1,-1) |    /(+1,+1,-1)
			//           |  /            |   /
			//           | /             |  /
			//           |/              | /
			//           /_______________|/ 
			//        (-1,-1,-1)       (+1,-1,-1)
			// 

			Vertex CubeVerts[8] =
			{
				{Math::Vector3(-1,-1,-1), Math::Vector4(1,0,0,1)},
				{Math::Vector3(-1,+1,-1), Math::Vector4(0,1,0,1)},
				{Math::Vector3(+1,+1,-1), Math::Vector4(0,0,1,1)},
				{Math::Vector3(+1,-1,-1), Math::Vector4(1,1,1,1)},

				{Math::Vector3(-1,-1,+1), Math::Vector4(1,1,0,1)},
				{Math::Vector3(-1,+1,+1), Math::Vector4(0,1,1,1)},
				{Math::Vector3(+1,+1,+1), Math::Vector4(1,0,1,1)},
				{Math::Vector3(+1,-1,+1), Math::Vector4(0.2f,0.2f,0.2f,1)},
			};
			// Create vertex buffer that stores cube vertices
			BufferDesc VertBuffDesc;
			VertBuffDesc.Name = "Cube vertex buffer";
			VertBuffDesc.Usage = USAGE_STATIC;
			VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
			VertBuffDesc.uiSizeInBytes = sizeof(CubeVerts);
			BufferData VBData;
			VBData.pData = CubeVerts;
			VBData.DataSize = sizeof(CubeVerts);
			Graphics::Context::GetDevice()->CreateBuffer(VertBuffDesc, VBData, &m_CubeVertexBuffer);
		}

		{
			// Indices
			Uint32 Indices[] =
			{
				2,0,1, 2,3,0,
				4,6,5, 4,7,6,
				0,7,4, 0,3,7,
				1,0,4, 1,4,5,
				1,5,2, 5,6,2,
				3,6,7, 3,2,6
			};
			// Create index buffer
			BufferDesc IndBuffDesc;
			IndBuffDesc.Name = "Cube index buffer";
			IndBuffDesc.Usage = USAGE_STATIC;
			IndBuffDesc.BindFlags = BIND_INDEX_BUFFER;
			IndBuffDesc.uiSizeInBytes = sizeof(Indices);
			BufferData IBData;
			IBData.pData = Indices;
			IBData.DataSize = sizeof(Indices);
			Graphics::Context::GetDevice()->CreateBuffer(IndBuffDesc, IBData, &m_CubeIndexBuffer);
		}
	}

	void OnMouseMovement(int xpos_a, int ypos_a) override
	{


	}

	void Update(float deltatime) override
	{
		const bool IsGL = Graphics::Context::GetDevice()->GetDeviceCaps().IsGLDevice();

		// Set cube world view matrix
		float4x4 CubeWorldView = rotationY(-static_cast<float >(deltatime) * 1.0f) * rotationX(PI_F*0.1f) *
			translationMatrix(0.f, 0.0f, 5.0f);
		float NearPlane = 0.1f;
		float FarPlane = 100.f;
		float aspectRatio = static_cast<float >(Graphics::Context::GetSwapChain()->GetDesc().Width) / static_cast<float >(Graphics::Context::GetSwapChain()->GetDesc().Height);
		// Projection matrix differs between DX and OpenGL
		auto Proj = Projection(PI_F / 4.f, aspectRatio, NearPlane, FarPlane, IsGL);
		// Compute world-view-projection matrix
		m_WorldViewProjMatrix = CubeWorldView * Proj;
	}
	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };
		Graphics::Context::GetContext()->ClearRenderTarget(nullptr, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(nullptr, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		{
			// Map the buffer and write current world-view-projection matrix
			MapHelper<float4x4> CBConstants(Graphics::Context::GetContext(), m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
			*CBConstants = transposeMatrix(m_WorldViewProjMatrix);
		}

		// Bind vertex buffer
		Uint32 offset = 0;
		IBuffer *pBuffs[] = { m_CubeVertexBuffer };
		Graphics::Context::GetContext()->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
		Graphics::Context::GetContext()->SetIndexBuffer(m_CubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		// Set pipeline state
		Graphics::Context::GetContext()->SetPipelineState(m_pPSO);
		// Commit shader resources
		// RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode makes sure
		// that resources are transitioned to proper states
		Graphics::Context::GetContext()->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		DrawAttribs DrawAttrs;
		DrawAttrs.IsIndexed = true; // This is an indexed draw call
		DrawAttrs.IndexType = VT_UINT32; // Index type
		DrawAttrs.NumIndices = 36;
		// Verify the state of vertex and index buffers
		DrawAttrs.Flags = DRAW_FLAG_VERIFY_STATES;
		Graphics::Context::GetContext()->Draw(DrawAttrs);

		Graphics::Context::GetSwapChain()->Present();
	}
};