#include "Components\Skybox.h"
#include <Core\Context.h>
#include <API\ShaderCompiler.h>
#include <AssetManager\AssetManager.h>
#include <API\RenderStates_Types.h>

namespace NuclearEngine
{
	namespace Components
	{

		float skyboxVertices[] = {
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f, 
			-1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f, // 0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, // 1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, // 1.0f, 1.0f,
			1.0f,  1.0f,  1.0f, // 1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f, // 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  //0.0f, 0.0f,

			-1.0f,  1.0f,  1.0f,  //1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, // 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, // 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, // 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, // 0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, // 1.0f, 0.0f,

			1.0f,  1.0f,  1.0f, // 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f, // 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f, // 0.0f, 1.0f,
			1.0f, -1.0f, -1.0f, // 0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, // 0.0f, 0.0f,
			1.0f,  1.0f,  1.0f,  //1.0f, 0.0f,

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

		void Skybox::Create(Components::GenericCamera * CameraCbuffer,const std::array<API::Texture_Data, 6>& data)
		{
			API::ShaderDesc desc;
			desc.Name = "SkyBox";
			API::CompileShader(&desc.VertexShaderCode, Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl").c_str(), API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
			API::CompileShader(&desc.PixelShaderCode, Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl").c_str(), API::ShaderType::Pixel, API::ShaderLanguage::HLSL);


			API::Shader::Create(&shader, &desc);

			_CameraCbuffer = CameraCbuffer;

			shader.SetConstantBuffer(&_CameraCbuffer->GetCBuffer(), API::ShaderType::Vertex);

			API::VertexBufferDesc VDesc;
			VDesc.data = skyboxVertices;
			VDesc.size = sizeof(skyboxVertices);
			VDesc.usage = API::BufferUsage::Static;

			API::VertexBuffer::Create(&vertexBuffer, VDesc);

			API::InputLayout vertexBufferLayout;
			vertexBufferLayout.AppendAttribute("POSITION", 0, API::DataType::Float3);
			vertexBuffer.SetInputLayout(&vertexBufferLayout, &shader);

			API::Texture_Desc Desc;
			Desc.Format = API::Format::R8G8B8A8_UNORM;
			Desc.Type = API::TextureType::TextureCube;
			Desc.GenerateMipMaps = false;
			API::Texture::Create(&texcube, data, Desc);

			API::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = API::TextureFilter::Linear2D;
			Samplerdesc.WrapU = API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapV = API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapW = API::TextureWrap::ClampToEdge;

			API::Sampler::Create(&mSampler, Samplerdesc);

			API::DepthStencilStateDesc DS_State;
			DS_State.DepthEnabled = true;
			DS_State.DepthFunc = API::Comparison_Func::LESS_EQUAL;
			DS_State.DepthMaskEnabled = false;
			API::DepthStencilState::Create(&cubemapstate, DS_State);
		}

		void Skybox::Create(Components::GenericCamera * CameraCbuffer, const std::array<std::string, 6>& paths)
		{
			API::Texture_Desc Desc;
			Desc.Format = API::Format::R8G8B8A8_UNORM;
			Desc.Type = API::TextureType::Texture2D;
			this->Create(CameraCbuffer, AssetManager::LoadTextureCubeFromFile(paths, Desc));
		}

		void Skybox::Render()
		{
			vertexBuffer.Bind();
			cubemapstate.Bind();
			shader.Bind();
			texcube.PSBind(0);
			mSampler.PSBind(0);
			Core::Context::Draw(36);
		}
	}
}