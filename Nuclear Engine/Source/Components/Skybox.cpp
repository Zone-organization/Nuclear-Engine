#include "Components\Skybox.h"
#include <Core\Context.h>
#include <API\ShaderCompiler.h>
#include <Managers\AssetManager.h>
#include <API\Buffer_Types.h>
#include <API\RenderStates_Types.h>
#include <API/InputLayout.h>

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

		void Skybox::Create(Skybox* skybox,Components::GenericCamera * CameraCbuffer,const std::array<API::Texture_Data, 6>& data)
		{
			API::VertexShader::Create(&skybox->v_shader, &API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl").c_str(), API::ShaderType::Vertex, API::ShaderLanguage::HLSL));
			API::PixelShader::Create(&skybox->p_shader, &API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl").c_str(), API::ShaderType::Pixel, API::ShaderLanguage::HLSL));

			skybox->m_camera = CameraCbuffer;

			skybox->v_shader.SetConstantBuffer(&skybox->m_camera->GetCBuffer());

			API::VertexBufferDesc VDesc;
			VDesc.data = skyboxVertices;
			VDesc.size = sizeof(skyboxVertices);
			VDesc.usage = API::BufferUsage::Static;

			API::VertexBuffer::Create(&skybox->m_vb, VDesc);

			API::InputLayout vertexBufferLayout;
			vertexBufferLayout.AppendAttribute("POSITION", 0, API::DataType::Float3);
			skybox->m_vb.SetInputLayout(&vertexBufferLayout, &skybox->v_shader);

			API::Texture_Desc Desc;
			Desc.Format = API::Format::R8G8B8A8_UNORM;
			Desc.Type = API::TextureType::TextureCube;
			Desc.GenerateMipMaps = false;
			API::Texture::Create(&skybox->m_texcube, data, Desc);

			API::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = API::TextureFilter::Linear2D;
			Samplerdesc.WrapU = API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapV = API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapW = API::TextureWrap::ClampToEdge;

			API::Sampler::Create(&skybox->m_sampler, Samplerdesc);

			API::DepthStencilStateDesc DS_State;
			DS_State.DepthEnabled = true;
			DS_State.DepthFunc = API::Comparison_Func::LESS_EQUAL;
			DS_State.DepthMaskEnabled = false;
			API::DepthStencilState::Create(&skybox->m_ds_state, DS_State);
		}

		void Skybox::Create(Skybox* skybox, Components::GenericCamera * CameraCbuffer, const std::array<std::string, 6>& paths)
		{
			API::Texture_Desc Desc;
			Desc.Format = API::Format::R8G8B8A8_UNORM;
			Desc.Type = API::TextureType::Texture2D;
			Skybox::Create(skybox, CameraCbuffer, Managers::AssetManager::LoadTextureCubeFromFile(paths, Desc));
		}
	}
}