#include <Engine\Graphics\NeoPipeline.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>
#include <Base/Utilities/Hash.h>

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

		std::vector<KeyChain> NeoPipeline::Create(const NeoPipelineDesc& Desc)
		{
			/*
			W.I.P
			*/

			std::string VShaderSource = Core::FileSystem::LoadShader(Desc.mVShaderPath, Desc.mConstDefines);
			std::string PShaderSource = Core::FileSystem::LoadShader(Desc.mPShaderPath, Desc.mConstDefines);
			std::vector<std::string> RequiredInstances;
			//Phase 1: Process Keychains
			for (auto KeyChain : Desc.mKeyChain)
			{
				for (auto Key : KeyChain)
				{

				}
			}

			//Phase 2: Create Pipelines
					//Uint32 KeyHash = Utilities::Hash(Key.KeyName);
					//RefCntAutoPtr<IShader> VShader;
					//RefCntAutoPtr<IShader> PShader;
					//RefCntAutoPtr<IPipelineState> Pipeline;

					//VShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(VShaderSource, Key.KeyPrefix + Key.KeyName), SHADER_TYPE_VERTEX);
					//PShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(PShaderSource, Key.KeyPrefix + Key.KeyName), SHADER_TYPE_PIXEL);

					//PipelineStateDesc PSODesc;
					//PSODesc.Name = std::string(Desc.mName + "_KEY_" + Key.KeyName).c_str();
					//PSODesc.IsComputePipeline = false;
					//PSODesc.GraphicsPipeline = Desc.GraphicsPipeline;
					//PSODesc.GraphicsPipeline.pVS = VShader;
					//PSODesc.GraphicsPipeline.pPS = PShader;
					//auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
					//Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&Pipeline, PSODesc, Vars, true);

					//PipelineWithSRB GeneratedPSO;
					//GeneratedPSO.PSO = Pipeline;
					//if (Key.CreateSRB)
					//{
					//	RefCntAutoPtr<IShaderResourceBinding> SRB;
					//	Pipeline->CreateShaderResourceBinding(&SRB, Key.InitSRBStaticResources);
					//	GeneratedPSO.SRB = SRB;
					//}
					//mPipelineStates[KeyHash] = GeneratedPSO;

			//return true;
		}
		Uint32 NeoPipeline::GetHashedKey(const std::string Key)
		{
			auto KeyHash = Utilities::Hash(Key);
			for (auto i : mPipelineStates)
			{
				if (i.first == KeyHash)
				{
					return KeyHash;
				}
			}

			//Error
			return 0;
		}
		PipelineWithSRB NeoPipeline::GetPipeline(Uint32 Key)
		{
			for (auto i : mPipelineStates)
			{
				if (i.first == Key)
				{
					return i.second;
				}
			}

			return PipelineWithSRB();
		}
	}
}