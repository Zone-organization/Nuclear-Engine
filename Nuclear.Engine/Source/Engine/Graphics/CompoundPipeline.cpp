#include <Engine\Graphics\CompoundPipeline.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Core\FileSystem.h>
#include <Core/Utilities/Hash.h>
#include <Core\Logger.h>

namespace Nuclear
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

		CompoundPipeline::CompoundPipeline()
		{
		}

		CompoundPipeline::~CompoundPipeline()
		{
			mKeyChain.clear();
			mVariants.clear();
		}

		std::vector<PipelineInstanceInfo> CompoundPipeline::Create(const CompoundPipelineDesc& Desc)
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
			
			if (VShaderSource == "" || PShaderSource == "")
			{
				NUCLEAR_ERROR("[CompoundPipeline] Couldn't Load Shaders!");
				return std::vector<PipelineInstanceInfo>();
			}
			for (auto Info : InstancesInfo)
			{
				RefCntAutoPtr<IShader> VShader;
				RefCntAutoPtr<IShader> PShader;
				RefCntAutoPtr<IPipelineState> Pipeline;

				GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(VShaderSource, Info.Defines), VShader.RawDblPtr(), SHADER_TYPE_VERTEX);
				GraphicsEngine::GetShaderManager()->CreateShader(MergeCode(PShaderSource, Info.Defines), PShader.RawDblPtr(), SHADER_TYPE_PIXEL);

				GraphicsPipelineStateCreateInfo PSOCreateInfo;
				std::string psoname(Desc.mName + "_ID_" + std::to_string(Info.mHashKey));
				PSOCreateInfo.PSODesc.Name = psoname.c_str();
				PSOCreateInfo.GraphicsPipeline = Desc.GraphicsPipeline;
				PSOCreateInfo.pVS = VShader;
				PSOCreateInfo.pPS = PShader;

				

				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VShader, PShader);
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&Pipeline, PSOCreateInfo, Vars, true);

				PipelineVariant GeneratedPSO;
				GeneratedPSO.PSO = Pipeline;
				if (Info.CreateSRB)
				{
					RefCntAutoPtr<IShaderResourceBinding> SRB;
					Pipeline->CreateShaderResourceBinding(&SRB, Info.InitStaticResources);
					GeneratedPSO.SRB = SRB;
				}
				mVariants[Info.mHashKey] = GeneratedPSO;
			}

			return InstancesInfo;
		}
		Uint32 CompoundPipeline::GetHashedKey(const std::string& Key)
		{
			auto KeyHash = Utilities::Hash(Key);
			for (auto i : mVariants)
			{
				if (i.first == KeyHash)
				{
					return KeyHash;
				}
			}

			//Error
			return 0;
		}
		PipelineVariant CompoundPipeline::GetVariant(Uint32 Key)
		{
			for (auto i : mVariants)
			{
				if (i.first == Key)
				{
					return i.second;
				}
			}

			NUCLEAR_ERROR("[CompoundPipeline] Variant: '{0}' Not Found!" , Key);
			return mVariants.begin()->second;
		}
	}
}