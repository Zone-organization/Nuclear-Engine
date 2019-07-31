#include <Engine\Graphics\NeoPipeline.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>
#include <Base/Utilities/Hash.h>
namespace NuclearEngine
{
	namespace Graphics
	{

		std::string MergeCode(std::string shadersource, std::set<std::string> defines)
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

		NeoPipeline::NeoPipeline()
		{
		}

		std::vector<PipelineInstanceInfo> NeoPipeline::Create(const NeoPipelineDesc& Desc)
		{
			std::vector<PipelineInstanceInfo> InstancesInfo;

			//Phase 1: Process Switches
			for (Uint32 ISwitch = 0; ISwitch < Desc.Switches.size(); ISwitch++)
			{
				for (Uint32 NextSwitch = 0; NextSwitch < Desc.Switches.size(); NextSwitch++)
				{
					for (Uint32 NextSwitch2 = 0; NextSwitch2 < Desc.Switches.size(); NextSwitch2++)
					{
						PipelineInstanceInfo Info_;
						Info_.Defines.insert(Desc.Switches.at(ISwitch).KeyPrefix + Desc.Switches.at(ISwitch).KeyName);
						Info_.Defines.insert(Desc.Switches.at(NextSwitch).KeyPrefix + Desc.Switches.at(NextSwitch).KeyName);
						Info_.Defines.insert(Desc.Switches.at(NextSwitch2).KeyPrefix + Desc.Switches.at(NextSwitch2).KeyName);
						
						std::string Key;
						for (auto i : Info_.Defines)
						{
							Key = Key + i;
						}
						Info_.mHashKey = Utilities::Hash(Key);
						int Found = 0;

						for (auto i : InstancesInfo)
						{
							if (i.mHashKey == Info_.mHashKey)
							{
								Found = Found + 1;
							}
						}

						if (Found == 0)
						{
							InstancesInfo.push_back(Info_);
						}
					}
				}
			}

			//Phase 2: Create Pipelines
			std::string VShaderSource = Core::FileSystem::LoadShader(Desc.mVShaderPath, Desc.mVSConstDefines);
			std::string PShaderSource = Core::FileSystem::LoadShader(Desc.mPShaderPath, Desc.mPSConstDefines);
			
			if (VShaderSource == "NoString" || PShaderSource == "NoString")
			{
				Log.Error("[NeoPipeline] Couldn't Load Shaders!\n");
				return std::vector<PipelineInstanceInfo>();
			}
			for (auto Info : InstancesInfo)
			{
				//Uint32 KeyHash = Utilities::Hash(Key.KeyName);
				RefCntAutoPtr<IShader> VShader;
				RefCntAutoPtr<IShader> PShader;
				RefCntAutoPtr<IPipelineState> Pipeline;

				VShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(VShaderSource, Info.Defines), SHADER_TYPE_VERTEX);
				PShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(PShaderSource, Info.Defines), SHADER_TYPE_PIXEL);

				PipelineStateDesc PSODesc;
				PSODesc.Name = std::string(Desc.mName + "_ID_" + std::to_string(Info.mHashKey)).c_str();
				PSODesc.IsComputePipeline = false;
				PSODesc.GraphicsPipeline = Desc.GraphicsPipeline;
				PSODesc.GraphicsPipeline.pVS = VShader;
				PSODesc.GraphicsPipeline.pPS = PShader;
				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&Pipeline, PSODesc, Vars, true);

				PipelineWithSRB GeneratedPSO;
				GeneratedPSO.PSO = Pipeline;
				if (Info.CreateSRB)
				{
					RefCntAutoPtr<IShaderResourceBinding> SRB;
					Pipeline->CreateShaderResourceBinding(&SRB, Info.InitStaticResources);
					GeneratedPSO.SRB = SRB;
				}
				mPipelineStates[Info.mHashKey] = GeneratedPSO;
			}

			return InstancesInfo;
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