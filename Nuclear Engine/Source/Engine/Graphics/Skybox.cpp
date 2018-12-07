#include "Engine\Graphics\Skybox.h"
#include <Engine\Graphics\API\Context.h>
#include <Engine/Graphics/API/ShaderCompiler.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine/Graphics/API/Buffer_Types.h>
#include <Engine/Graphics/API/RenderStates_Types.h>
#include <Engine/Graphics/API/VertexFormat.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine/Graphics/API/IndexBuffer.h>
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

		void Skybox::Initialize(Components::CameraComponent * Camera, LLGL::Texture* data)
		{
			LLGL::Shader*::Create(&mVShader, LLGL::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl").c_str(), LLGL::ShaderType::Vertex));
			LLGL::Shader*::Create(&mPShader, LLGL::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl").c_str(), LLGL::ShaderType::Pixel));

			mVShader.SetConstantBuffer(&Camera->GetCBuffer());

			LLGL::BufferDescriptor VDesc;
			VDesc.Data = skyboxVertices;
			VDesc.Size = sizeof(skyboxVertices);
			VDesc.UsageType = LLGL::BufferUsage::Static;

			LLGL::Buffer*::Create(&mVBuffer, VDesc);

			LLGL::VertexFormat vertexBufferLayout;
			vertexBufferLayout.AppendAttribute("POSITION", 0, LLGL::Format::RGB32Float);
			mVBuffer.SetVertexFormat(&vertexBufferLayout, &mVShader);

			LLGL::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = LLGL::TextureFilter::Linear2D;
			Samplerdesc.WrapU = LLGL::TextureWrap::ClampToEdge;
			Samplerdesc.WrapV = LLGL::TextureWrap::ClampToEdge;
			Samplerdesc.WrapW = LLGL::TextureWrap::ClampToEdge;

			LLGL::Sampler::Create(&mSampler, Samplerdesc);

			LLGL::DepthStencilStateDesc DS_State;
			DS_State.DepthEnabled = true;
			DS_State.DepthFunc = LLGL::Comparison_Func::LESS_EQUAL;
			DS_State.DepthMaskEnabled = false;
			LLGL::DepthStencilState::Create(&mDSState, DS_State);
		}

		void Skybox::Initialize(Components::CameraComponent * Camera, const std::array<LLGL::Texture_Data, 6>& data)
		{
			ReleaseTex = true;
			mTexture.Delete(&mTexture);

			LLGL::Texture_Desc Desc;
			Desc.Format = LLGL::Format::R8G8B8A8_UNORM;
			Desc.Type = LLGL::TextureType::TextureCube;
			Desc.GenerateMipMaps = false;
			LLGL::Texture::Create(&mTexture, data, Desc);
			return Initialize(Camera, mTexture);
		}
		void Skybox::Initialize(Components::CameraComponent * Camera, const std::array<std::string, 6>& paths)
		{
			ReleaseTex = true;
			LLGL::Texture_Desc Desc;
			Desc.Format = LLGL::Format::R8G8B8A8_UNORM;
			Desc.Type = LLGL::TextureType::Texture2D;
			return Initialize(Camera, Managers::AssetManager::LoadTextureCubeFromFile(paths, Desc));
		}

		void Skybox::Release()
		{
			LLGL::Shader*::Delete(&mVShader);
			LLGL::Shader*::Delete(&mPShader);
			LLGL::Buffer*::Delete(&mVBuffer);
			LLGL::Sampler::Delete(&mSampler);
			LLGL::DepthStencilState::Delete(&mDSState);
			if (ReleaseTex)
			{
				LLGL::Texture::Delete(&mTexture);
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
			LLGL::Context::Draw(36);
		}

	}
}