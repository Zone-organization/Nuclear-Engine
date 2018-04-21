#include "Graphics\Skybox.h"
#include <Graphics\API\Context.h>
#include <Graphics/API/ShaderCompiler.h>
#include <Managers\AssetManager.h>
#include <Graphics/API/Buffer_Types.h>
#include <Graphics/API/RenderStates_Types.h>
#include <Graphics/API/InputLayout.h>

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

		void Skybox::Initialize(Components::CameraComponent * Camera, Graphics::API::Texture data)
		{
			Graphics::API::VertexShader::Create(&mVShader, &Graphics::API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl").c_str(), Graphics::API::ShaderType::Vertex));
			Graphics::API::PixelShader::Create(&mPShader, &Graphics::API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl").c_str(), Graphics::API::ShaderType::Pixel));

			mVShader.SetConstantBuffer(&Camera->GetCBuffer());

			Graphics::API::VertexBufferDesc VDesc;
			VDesc.data = skyboxVertices;
			VDesc.size = sizeof(skyboxVertices);
			VDesc.usage = Graphics::API::BufferUsage::Static;

			Graphics::API::VertexBuffer::Create(&mVBuffer, VDesc);

			Graphics::API::InputLayout vertexBufferLayout;
			vertexBufferLayout.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
			mVBuffer.SetInputLayout(&vertexBufferLayout, &mVShader);

			Graphics::API::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = Graphics::API::TextureFilter::Linear2D;
			Samplerdesc.WrapU = Graphics::API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapV = Graphics::API::TextureWrap::ClampToEdge;
			Samplerdesc.WrapW = Graphics::API::TextureWrap::ClampToEdge;

			Graphics::API::Sampler::Create(&mSampler, Samplerdesc);

			Graphics::API::DepthStencilStateDesc DS_State;
			DS_State.DepthEnabled = true;
			DS_State.DepthFunc = Graphics::API::Comparison_Func::LESS_EQUAL;
			DS_State.DepthMaskEnabled = false;
			Graphics::API::DepthStencilState::Create(&mDSState, DS_State);
		}

		void Skybox::Initialize(Components::CameraComponent * Camera, const std::array<Graphics::API::Texture_Data, 6>& data)
		{
			ReleaseTex = true;
			mTexture.Delete(&mTexture);

			Graphics::API::Texture_Desc Desc;
			Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
			Desc.Type = Graphics::API::TextureType::TextureCube;
			Desc.GenerateMipMaps = false;
			Graphics::API::Texture::Create(&mTexture, data, Desc);
			return Initialize(Camera, mTexture);
		}
		void Skybox::Initialize(Components::CameraComponent * Camera, const std::array<std::string, 6>& paths)
		{
			ReleaseTex = true;
			Graphics::API::Texture_Desc Desc;
			Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
			Desc.Type = Graphics::API::TextureType::Texture2D;
			return Initialize(Camera, Managers::AssetManager::LoadTextureCubeFromFile(paths, Desc));
		}

		void Skybox::Release()
		{
			Graphics::API::VertexShader::Delete(&mVShader);
			Graphics::API::PixelShader::Delete(&mPShader);
			Graphics::API::VertexBuffer::Delete(&mVBuffer);
			Graphics::API::Sampler::Delete(&mSampler);
			Graphics::API::DepthStencilState::Delete(&mDSState);
			if (ReleaseTex)
			{
				Graphics::API::Texture::Delete(&mTexture);
			}
		}

		void Skybox::Render()
		{
			mVBuffer.Bind();
			mDSState.Bind();
			mVShader.Bind();
			mPShader.Bind();
			mTexture.PSBind(0);
			mSampler.PSBind(0);
			Graphics::API::Context::Draw(36);
		}

	}
}