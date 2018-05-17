#include <Engine\Graphics\CameraPostProcess.h>
#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		void CameraPostProcess::Bake(const PostProcessDesc& Desc)
		{
			Graphics::API::Texture_Desc ScreenTexDesc;
			if (Desc.HDR == true)
			{
				ScreenTexDesc.Format = Graphics::API::Format::R16G16B16A16_FLOAT;
			}
			else
			{
				ScreenTexDesc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
			}
			ScreenTexDesc.Type = Graphics::API::TextureType::Texture2D;
			ScreenTexDesc.GenerateMipMaps = false;

			Graphics::API::Texture_Data Data;
			Data.Img_Data_Buf = NULL;
			Data.Width = Desc.WindowWidth;
			Data.Height = Desc.WindowHeight;
			Graphics::API::Texture::Create(&PostProcessTexture, Data, ScreenTexDesc);

			Graphics::API::RenderTarget::Create(&PostProcessRT);
			PostProcessRT.AttachTexture(&PostProcessTexture);
			PostProcessRT.AttachDepthStencilBuffer(Math::Vector2ui(Desc.WindowWidth, Desc.WindowHeight));

			Managers::AutoVertexShaderDesc VertShaderDesc;
			VertShaderDesc.Use_Camera = false;
			VertShaderDesc.InNormals = false;
			VertShaderDesc.OutFragPos = false;

			PostProcess_VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
			Assets::Mesh::CreateScreenQuad(&PostProcessScreenQuad);
			PostProcessScreenQuad.Initialize(&PostProcess_VShader);
			std::vector<std::string> defines;
			if (Desc.GammaCorrection == true) { defines.push_back("NE_GAMMA_CORRECTION_ENABLED"); }
			if (Desc.HDR == true) { defines.push_back("NE_HDR_ENABLED"); }

			Graphics::API::PixelShader::Create(
				&PostProcess_PShader,
				&Graphics::API::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl", defines, std::vector<std::string>(), true),
					Graphics::API::ShaderType::Pixel));


			Graphics::API::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = Graphics::API::TextureFilter::Point2D;
			Graphics::API::Sampler::Create(&ScreenSampler, Samplerdesc);
		}
	}
}