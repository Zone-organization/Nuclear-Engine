#include "Engine\Graphics\Skybox.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine\Components\CameraComponent.h>
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

		void Skybox::Initialize(Components::CameraComponent * Camera, LLGL::Texture* data)
		{
			Graphics::ShaderCompiler::CompileAndCreateShader(&mVShader,
				Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl"),
				LLGL::ShaderType::Vertex);

			Graphics::ShaderCompiler::CompileAndCreateShader(&mPShader,
				Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl"),
				LLGL::ShaderType::Fragment);

			//mVShader.SetConstantBuffer(&Camera->GetCBuffer());

			LLGL::VertexFormat vertexBufferLayout;
			vertexBufferLayout.AppendAttribute({ "POSITION", LLGL::Format::RGB32Float });

			LLGL::BufferDescriptor VDesc;
			VDesc.bindFlags = LLGL::BindFlags::VertexBuffer;
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

			TextureLoadingDesc Desc;
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
			TextureLoadingDesc Desc;
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