#include <Engine\Graphics\DynamicPipelineState.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>

namespace NuclearEngine
{
	namespace Graphics
	{

		std::string MergeCode(std::string shadersource, std::string key)
		{
			std::vector<std::string> MergedCode;

			std::string firstLine = shadersource.substr(0, shadersource.find("\n"));

			std::string define = "#define " + key + "\n";
			MergedCode.push_back(define);

			MergedCode.push_back(key);
			std::string str;
			for (unsigned int i = 0; i < MergedCode.size(); ++i)
				str = str + MergedCode[i].c_str();

			return str;
		}

		bool DynamicPipelineState::Create(DynamicPipelineStateDesc& Desc)
		{
			std::string VShaderSource = Core::FileSystem::LoadShader(Desc.mVShaderPath, Desc.mConstDefines);
			std::string PShaderSource = Core::FileSystem::LoadShader(Desc.mPShaderPath, Desc.mConstDefines);


			for (auto Key : Desc.mKeys)
			{
				RefCntAutoPtr<IShader> VShader;
				RefCntAutoPtr<IShader> PShader;

				VShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(VShaderSource, Key), SHADER_TYPE_VERTEX);
				PShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(PShaderSource, Key), SHADER_TYPE_PIXEL);

				PipelineStateDesc PSODesc;
				PSODesc.Name = std::string(Desc.mName + "_KEY_" + Key).c_str();
				PSODesc.IsComputePipeline = false;
				PSODesc.GraphicsPipeline = Desc.GraphicsPipeline;
				PSODesc.GraphicsPipeline.pVS = VShader;
				PSODesc.GraphicsPipeline.pPS = PShader;
				//PSODesc.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
				//PSODesc.GraphicsPipeline.InputLayout.NumElements = LayoutElems.size();
				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
				//Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&mPipeline, PSODesc, Vars, true);
			}
			return false;
		}
		Uint32 DynamicPipelineState::GetHashedKey(const std::string Key)
		{
			return Uint32();
		}
		RefCntAutoPtr<IPipelineState> DynamicPipelineState::GetPipeline(Uint32 Key)
		{
			return RefCntAutoPtr<IPipelineState>();
		}
	}
}