#include "Components\Skybox.h"
#include <Graphics\API\Context.h>
#include <Graphics/API/ShaderCompiler.h>
#include <Managers\AssetManager.h>
#include <Graphics/API/Buffer_Types.h>
#include <Graphics/API/RenderStates_Types.h>
#include <Graphics/API/InputLayout.h>

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

		void Skybox::Create(Skybox* skybox,Components::GenericCamera * CameraCbuffer,const std::array<Graphics::API::Texture_Data, 6>& data)
		{
			Graphics::API::VertexShader::Create(&skybox->v_shader, &Graphics::API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl").c_str(), Graphics::API::ShaderType::Vertex));
			Graphics::API::PixelShader::Create(&skybox->p_shader, &Graphics::API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl").c_str(), Graphics::API::ShaderType::Pixel));

			skybox->m_camera = CameraCbuffer;

			skybox->v_shader.SetConstantBuffer(&skybox->m_camera->GetCBuffer());

			Graphics::API::VertexBufferDesc VDesc;
			VDesc.data = skyboxVertices;
			VDesc.size = sizeof(skyboxVertices);
			VDesc.usage = Graphics::API::BufferUsage::Static;

			Graphics::API::VertexBuffer::Create(&skybox->m_vb, VDesc);

			Graphics::API::InputLayout vertexBufferLayout;
			vertexBufferLayout.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
			skybox->m_vb.SetInputLayout(&vertexBufferLayout, &skybox->v_shader);

			Graphics::API::Texture_Desc Desc;
			Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
			Desc.Type = Graphics::API::TextureType::TextureCube;
			Desc.GenerateMipMaps = false;
			Graphics::API::Texture::Create(&skybox->m_texcube, data, Desc);

			Graphics::API::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = Graphics::API::TextureFilter::Linear2D;
			Samplerdesc.WrapU = Graphics::API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapV = Graphics::API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapW = Graphics::API::TextureWrap::ClampToEdge;

			Graphics::API::Sampler::Create(&skybox->m_sampler, Samplerdesc);

			Graphics::API::DepthStencilStateDesc DS_State;
			DS_State.DepthEnabled = true;
			DS_State.DepthFunc = Graphics::API::Comparison_Func::LESS_EQUAL;
			DS_State.DepthMaskEnabled = false;
			Graphics::API::DepthStencilState::Create(&skybox->m_ds_state, DS_State);
		}

		void Skybox::Create(Skybox* skybox, Components::GenericCamera * CameraCbuffer, const std::array<std::string, 6>& paths)
		{
			Graphics::API::Texture_Desc Desc;
			Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
			Desc.Type = Graphics::API::TextureType::Texture2D;
			Skybox::Create(skybox, CameraCbuffer, Managers::AssetManager::LoadTextureCubeFromFile(paths, Desc));
		}
	}
}