#include <Rendering/RenderPasses/AmbientOcclusionPass.h>
#include <Assets/AssetManager.h>
#include <Assets/Shader.h>
#include <Graphics/Context.h>

namespace Nuclear
{
	namespace Rendering
	{
		using namespace Diligent;
#define SAMPLES_NUM 64

		void AmbientOcclusionPass::Bake(Uint32 RTWidth, Uint32 RTHeight)
		{
			std::vector<LayoutElement> Layout;
			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));

			Assets::ShaderImportingDesc desc;
			desc.mCommonOptions.mLoadOnly = true;

			pSSAO_ExtractShader = Assets::AssetManager::GetInstance().Import<Assets::Shader>("@NuclearAssets@/Shaders/SSAO_Extract.NuclearShader");
		
			//bake pipeline
			Graphics::ShaderPipelineBakingDesc bakedesc;
			bakedesc.pVariantsFactory = &Graphics::GraphicsEngine::GetInstance().GetDefaultShaderPipelineVariantFactory();
			pSSAO_ExtractShader->GetShaderPipeline().Bake(bakedesc);

			//create cb
			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_SSAO_Samples";
			CBDesc.Size = (sizeof(Math::Vector3) * SAMPLES_NUM) + sizeof(Math::Matrix4);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &pSSAO_ExtractCB);

			//create rt
			Graphics::RenderTargetDesc rtdesc;
			rtdesc.mName = "SSAO_RT";
			rtdesc.mType = "AmbientOcclusion";
			rtdesc.ColorTexFormat = TEX_FORMAT_R32_FLOAT;
			rtdesc.mDimensions = Math::Vector2ui(RTWidth, RTHeight);
			mSSAO_RT.Create(rtdesc);

		}
		void AmbientOcclusionPass::Update(FrameRenderData* framedata)
		{
		}
		void AmbientOcclusionPass::ResizeRTs(Uint32 RTWidth, Uint32 RTHeight)
		{
			mSSAO_RT.Resize(Math::Vector2ui(RTWidth, RTHeight));
		}
	}
}