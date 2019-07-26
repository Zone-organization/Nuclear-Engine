#include <Engine\Graphics\NeoPipeline.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>
#include <Base/Utilities/Hash.h>

namespace NuclearEngine
{
	namespace Graphics
	{

		std::string MergeCode(std::string shadersource, std::vector<std::string> defines)
		{
			std::vector<std::string> MergedCode;

			std::string firstLine = shadersource.substr(0, shadersource.find("\n"));

			for (auto i : defines)
			{
				std::string define = "#define " + i + "\n";
				MergedCode.push_back(define);
			}
			std::string str;
			for (unsigned int i = 0; i < MergedCode.size(); ++i)
				str = str + MergedCode[i].c_str();

			return str;
		}

		struct PipelineInstanceInfo
		{
			std::vector<std::string> Defines;
		/*	Uint32 mHashKey = 0;
			bool CreateSRB;
			bool InitStaticResources;*/
		};

		std::vector<KeyChain> NeoPipeline::Create(const NeoPipelineDesc& Desc)
		{
			/*
			W.I.P
			*/

			std::string VShaderSource = Core::FileSystem::LoadShader(Desc.mVShaderPath, Desc.mConstDefines);
			std::string PShaderSource = Core::FileSystem::LoadShader(Desc.mPShaderPath, Desc.mConstDefines);
			std::vector<PipelineInstanceInfo> InstancesInfo;

			//Phase 1: Process Keychains

			//for (Uint32 IKey = 0; IKey < Desc.mKeyChains.at(0).size(); IKey++)
			//{
			//	PipelineInstanceInfo Info_;
			//	Info_.Defines.push_back(Desc.mKeyChains.at(0).at(IKey).KeyName);

			//	for (Uint32 INextKeyChain = 1; INextKeyChain < Desc.mKeyChains.size(); INextKeyChain++)
			//	{

			//		for (Uint32 INextKey = 0; INextKey < Desc.mKeyChains.at(INextKeyChain).size(); INextKey++)
			//		{

			//			Info_.Defines.push_back(Desc.mKeyChains.at(INextKeyChain).at(INextKey).KeyName);

			//		
			//			break;
			//		}


			//	}
			//	InstancesInfo.push_back(Info_);

			//}
			

			//Phase 2: Create Pipelines
			for (auto Info : InstancesInfo)
			{
				//Uint32 KeyHash = Utilities::Hash(Key.KeyName);
				RefCntAutoPtr<IShader> VShader;
				RefCntAutoPtr<IShader> PShader;
				RefCntAutoPtr<IPipelineState> Pipeline;

				VShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(VShaderSource, Info.Defines), SHADER_TYPE_VERTEX);
				PShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(PShaderSource, Info.Defines), SHADER_TYPE_PIXEL);

				//PipelineStateDesc PSODesc;
				//PSODesc.Name = std::string(Desc.mName + "_ID_" + std::to_string(Info.mHashKey)).c_str();
				//PSODesc.IsComputePipeline = false;
				//PSODesc.GraphicsPipeline = Desc.GraphicsPipeline;
				//PSODesc.GraphicsPipeline.pVS = VShader;
				//PSODesc.GraphicsPipeline.pPS = PShader;
				//auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
				//Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&Pipeline, PSODesc, Vars, true);

				//PipelineWithSRB GeneratedPSO;
				//GeneratedPSO.PSO = Pipeline;
				//if (Info.CreateSRB)
				//{
				//	RefCntAutoPtr<IShaderResourceBinding> SRB;
				//	Pipeline->CreateShaderResourceBinding(&SRB, Info.InitStaticResources);
				//	GeneratedPSO.SRB = SRB;
				//}
				//mPipelineStates[Info.mHashKey] = GeneratedPSO;
			}

			return std::vector<KeyChain>();
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