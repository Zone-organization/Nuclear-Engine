#include "Engine\Graphics\Skybox.h"
#include <Engine\Graphics\Context.h>
#include <Engine/Graphics/ShaderCompiler.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine\Components\CameraComponent.h>
#include <LLGL\Buffer.h>
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

			auto shadersource = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl");

			LLGL::ShaderDescriptor sdesc;
			sdesc.source = shadersource.c_str();
			sdesc.sourceSize = shadersource.size();
			sdesc.sourceType = LLGL::ShaderSourceType::CodeString;
			sdesc.type = LLGL::ShaderType::Vertex;
			mVShader.mShader = Graphics::Context::GetRenderer()->CreateShader(sdesc);

			shadersource = Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl");
			sdesc.type = LLGL::ShaderType::Fragment;
			mPShader.mShader = Graphics::Context::GetRenderer()->CreateShader(sdesc);

			//mVShader.SetConstantBuffer(&Camera->GetCBuffer());

			LLGL::VertexFormat vertexBufferLayout;
			vertexBufferLayout.AppendAttribute({ "POSITION", LLGL::Format::RGB32Float });

			LLGL::BufferDescriptor VDesc;
			VDesc.type = LLGL::BufferType::Vertex;
			VDesc.size = sizeof(skyboxVertices);
			VDesc.vertexBuffer.format = vertexBufferLayout;

			mVBuffer = Graphics::Context::GetRenderer()->CreateBuffer(VDesc, skyboxVertices);



			LLGL::SamplerDescriptor Samplerdesc;
			Samplerdesc.minFilter = Samplerdesc.minFilter = Samplerdesc.mipMapFilter = LLGL::SamplerFilter::Linear;
			Samplerdesc.addressModeU = LLGL::SamplerAddressMode::Clamp;
			Samplerdesc.addressModeV = LLGL::SamplerAddressMode::Clamp;
			Samplerdesc.addressModeW = LLGL::SamplerAddressMode::Clamp;

			mSampler = Graphics::Context::GetRenderer()->CreateSampler(Samplerdesc);

			mDepthState.testEnabled = true;
			mDepthState.writeEnabled = false;
			mDepthState.compareOp = LLGL::CompareOp::LessEqual;
		}

		void Skybox::Initialize(Components::CameraComponent * Camera, const std::array<LLGL::SrcImageDescriptor, 6>& data)
		{
			ReleaseTex = true;
			Graphics::Context::GetRenderer()->Release(*mTexture);

			LLGL::TextureDescriptor Desc;
			Desc.format = LLGL::Format::RGBA8UNorm;
			Desc.type = LLGL::TextureType::TextureCube;
			//Desc.GenerateMipMaps = false;

			mTexture = Graphics::Context::GetRenderer()->CreateTexture(Desc);


			//LLGL_FIXME
			//LLGL::Texture::Create(&mTexture, data, Desc);


			return Initialize(Camera, mTexture);
		}
		void Skybox::Initialize(Components::CameraComponent * Camera, const std::array<std::string, 6>& paths)
		{
			ReleaseTex = true;
			LLGL::TextureDescriptor Desc;
			Desc.format = LLGL::Format::RGBA8UNorm;
			Desc.type = LLGL::TextureType::Texture2D;
			return Initialize(Camera, Managers::AssetManager::LoadTextureCubeFromFile(paths, Desc));
		}

		void Skybox::Release()
		{
			Graphics::Context::GetRenderer()->Release(*mVShader.mShader);
			Graphics::Context::GetRenderer()->Release(*mPShader.mShader);
			Graphics::Context::GetRenderer()->Release(*mVBuffer);
			Graphics::Context::GetRenderer()->Release(*mSampler);

			if (ReleaseTex)
			{
				Graphics::Context::GetRenderer()->Release(*mTexture);
			}
		}

		void Skybox::Render()
		{
		/*	mVBuffer.Bind();
			mDSState.Bind();
			mVShader.Bind();
			mPShader.Bind();
			mTexture.PSBind(0);
			mSampler.PSBind(0);
			LLGL::Context::Draw(36);*/
		}

	}
}