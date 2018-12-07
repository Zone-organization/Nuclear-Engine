#include <Engine\Graphics\CameraPostProcess.h>
#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		void CameraPostProcess::Bake(const PostProcessDesc& Desc)
		{
			LLGL::TextureDescriptor ScreenTexDesc;
			if (Desc.HDR == true)
			{
				ScreenTexDesc.Format = LLGL::Format::R16G16B16A16_FLOAT;
			}
			else
			{
				ScreenTexDesc.Format = LLGL::Format::R8G8B8A8_UNORM;
			}
			ScreenTexDesc.Type = LLGL::TextureType::Texture2D;
			ScreenTexDesc.GenerateMipMaps = false;

			LLGL::SrcImageDescriptor Data;
			Data.Img_Data_Buf = NULL;
			Data.Width = Desc.WindowWidth;
			Data.Height = Desc.WindowHeight;
			LLGL::Texture::Create(&PostProcessTexture, Data, ScreenTexDesc);

			LLGL::RenderTarget::Create(&PostProcessRT);
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

			LLGL::Shader*::Create(
				&PostProcess_PShader,
				LLGL::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl", defines, std::vector<std::string>(), true),
					LLGL::ShaderType::Pixel));


			LLGL::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = LLGL::TextureFilter::Point2D;
			LLGL::Sampler::Create(&ScreenSampler, Samplerdesc);
		}
	}
}