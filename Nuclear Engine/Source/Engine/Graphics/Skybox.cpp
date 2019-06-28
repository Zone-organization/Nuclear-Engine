#include "Engine\Graphics\Skybox.h"
#include <Engine\Graphics\Context.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine\Components\CameraComponent.h>
#include "Engine\Graphics\GraphicsEngine.h"
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

		void Skybox::Initialize(Components::CameraComponent* Camera, const std::array<Assets::Image, 6> & data)
		{
			TextureDesc TexDesc;
			TexDesc.Name = "SkyBox_TextureCube";
			TexDesc.Type = RESOURCE_DIM_TEX_CUBE;
			TexDesc.Usage = USAGE_DEFAULT;
			TexDesc.BindFlags = BIND_SHADER_RESOURCE;
			TexDesc.Width = data.at(0).mWidth;
			TexDesc.Height = data.at(0).mHeight;
			TexDesc.Format = TEX_FORMAT_RGBA8_UNORM;
			TexDesc.ArraySize = 6;
			TexDesc.MipLevels = 1;

			RefCntAutoPtr<ITexture> TexCube;
			TextureData Data;


			TextureSubResData subData[6];
			for (unsigned int i = 0; i < data.size(); i++)
			{
				subData[i].pData = data[i].mData;
				subData[i].Stride = data[i].mWidth * 4;
			}

			Data.NumSubresources = 6;
			Data.pSubResources = &subData[0];
			Graphics::Context::GetDevice()->CreateTexture(TexDesc, &Data, &TexCube);
			mTextureSRV = TexCube->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

			/*Graphics::GraphicsEngine::GetShaderManager()->CompileAndCreateShader(&mVShader,
				Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.vs.hlsl"),
				LLGL::ShaderType::Vertex);

			Graphics::ShaderCompiler::CompileAndCreateShader(&mPShader,
				Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Skybox.ps.hlsl"),
				LLGL::ShaderType::Fragment);

			mVShader.SetConstantBuffer(&Camera->GetCBuffer());

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
			mDepthState.compareOp = LLGL::CompareOp::LessEqual;*/

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