#include "Components\Skybox.h"
#include <Core\Context.h>

namespace NuclearEngine
{
	namespace Components
	{

		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};


		Skybox::Skybox(Components::GenericCamera* CameraCbuffer, std::array<NuclearEngine::Texture_Data, 6> data)
		{
			shader = new API::Shader("Skybox",
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));

			_CameraCbuffer = CameraCbuffer;
			vertexBufferLayout = new API::InputLayout();			
			unsigned int slot = shader->GetUniformBufferSlot(CameraCbuffer->GetCBuffer(), ShaderType::Vertex);
			shader->SetUniformBuffer(CameraCbuffer->GetCBuffer(), slot, ShaderType::Vertex);

			API::VertexBufferDesc VDesc;
			VDesc.data = skyboxVertices;
			VDesc.size = sizeof(skyboxVertices);
			VDesc.usage = BufferGPUUsage::Static;
			VDesc.accessflag = BufferCPUAccess::Default;

			vertexBuffer = new API::VertexBuffer(VDesc);

			vertexBufferLayout->Push("POSITION",0 , DataType::Float3);
			vertexBuffer->SetInputLayout(vertexBufferLayout, shader);

			Texture_Desc Desc;
			Desc.Format = TextureFormat::R8G8B8A8;
			Desc.Wrap = TextureWrap::ClampToEdge;
			Desc.Filter = TextureFilter::Linear2D;
			texcube = new API::Texture(data, Desc);

			DepthStencilStateDesc DS_State;
			DS_State.DepthStencilEnabled = true;
			DS_State.DepthFunc = Comparison_Func::LESS_EQUAL;

			cubemapstate = new API::DepthStencilState(DS_State);
		}
		Skybox::~Skybox()
		{
		}

		void Skybox::Render()
		{
			//Math::Matrix4 view;
			

			//_CameraCbuffer->SetViewMatrix(view);
			//_CameraCbuffer->Update();
			vertexBuffer->Bind();
			cubemapstate->Bind();
			shader->Bind();
			texcube->PSBind("NE_SkyboxTexture",shader,0);
			Core::Context::Draw(36);
			shader->Unbind();
			cubemapstate->Unbind();
			vertexBuffer->Unbind();
		}
	}
}