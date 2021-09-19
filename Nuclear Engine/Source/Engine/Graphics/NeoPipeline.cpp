#include <Engine\Graphics\NeoPipeline.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>
#include <Base/Utilities/Hash.h>
namespace NuclearEngine
{
	namespace Graphics
	{

		std::string MergeCode(const std::string& shadersource, const std::set<std::string>& defines)
		{
			std::vector<std::string> MergedCode;

			for (auto i : defines)
			{
				MergedCode.push_back("#define " + i + "\n");
			}
			std::string str;
			MergedCode.push_back(shadersource);
			for (auto i : MergedCode)
			{
				str = str + i;
			}
			return str;
		}

		NeoPipeline::NeoPipeline()
		{
		}

		NeoPipeline::~NeoPipeline()
		{
		}

		std::vector<PipelineInstanceInfo> NeoPipeline::Create(const NeoPipelineDesc& Desc)
		{
			std::vector<PipelineInstanceInfo> InstancesInfo;

			PipelineInstanceInfo ZeroInstance;
			ZeroInstance.mHashKey = 0;
			ZeroInstance.CreateSRB = Desc.ZeroInstanceCreateSRB;
			ZeroInstance.InitStaticResources = Desc.ZeroInstanceInitSRBStaticResources;

			InstancesInfo.push_back(ZeroInstance);
			
			//Phase 1: Process Switches
			for (Uint32 ISwitch = 0; ISwitch < Desc.Switches.size(); ISwitch++)
			{
				for (Uint32 NextSwitch = 0; NextSwitch < Desc.Switches.size(); NextSwitch++)
				{
					for (Uint32 NextSwitch2 = 0; NextSwitch2 < Desc.Switches.size(); NextSwitch2++)
					{
						PipelineInstanceInfo Info_;
						Info_.Defines.insert(Desc.Switches.at(ISwitch).KeyName);
						Info_.Defines.insert(Desc.Switches.at(NextSwitch).KeyName);
						Info_.Defines.insert(Desc.Switches.at(NextSwitch2).KeyName);
						Info_.CreateSRB = Desc.Switches.at(ISwitch).CreateSRB;
						Info_.InitStaticResources = Desc.Switches.at(ISwitch).InitSRBStaticResources;

						for (auto i : Info_.Defines)
						{
							auto iHash = Utilities::Hash(i);
							Info_.mHashKey = Info_.mHashKey + iHash;
						}

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
				RefCntAutoPtr<IShader> VShader;
				RefCntAutoPtr<IShader> PShader;
				RefCntAutoPtr<IPipelineState> Pipeline;

				VShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(VShaderSource, Info.Defines), SHADER_TYPE_VERTEX);
				PShader = GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(PShaderSource, Info.Defines), SHADER_TYPE_PIXEL);

				GraphicsPipelineStateCreateInfo PSOCreateInfo;
				PSOCreateInfo.PSODesc.Name = std::string(Desc.mName + "_ID_" + std::to_string(Info.mHashKey)).c_str();
				PSOCreateInfo.GraphicsPipeline = Desc.GraphicsPipeline;
				PSOCreateInfo.pVS = VShader;
				PSOCreateInfo.pPS = PShader;

				

				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&Pipeline, PSOCreateInfo, Vars, true);

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