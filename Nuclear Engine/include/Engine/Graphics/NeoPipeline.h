#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <unordered_map>
#include <Core\Path.h>
#include <set>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct PipelineSwitch
		{
			PipelineSwitch(std::string KeyName_, std::string KeyPrefix_ = "NE_ENABLE_", bool CreateSRB_ = true, bool InitSRBStaticResources_ = true)
				: KeyName(KeyName_), KeyPrefix(KeyPrefix_), CreateSRB(CreateSRB_), InitSRBStaticResources(InitSRBStaticResources_)
			{

			}
			std::string KeyName;
			std::string KeyPrefix;
			bool CreateSRB;
			bool InitSRBStaticResources;

			//AUTO FILLED ON CREATING NEOPIPELINE
			Uint32 mHashedName = 0;
		};

		struct PipelineInstanceInfo
		{
			std::set<std::string> Defines;
			Uint32 mHashKey = 0;
			bool CreateSRB;
			bool InitStaticResources;
		};

		struct NeoPipelineDesc
		{
			std::string mName;

			Core::Path mVShaderPath;
			Core::Path mPShaderPath;

			std::vector<PipelineSwitch> Switches;
			std::vector<std::string> mConstDefines;

			//Increases memory cosumptions as it stores KeyChain information
			bool SaveKeyChainsInfo = DEBUG_TRUE_BOOL;

			GraphicsPipelineDesc GraphicsPipeline;
		};

		struct PipelineWithSRB
		{
			RefCntAutoPtr<IPipelineState> PSO;
			RefCntAutoPtr<IShaderResourceBinding> SRB;
		};

		class NEAPI NeoPipeline
		{
		public:
			NeoPipeline();
			std::vector<PipelineInstanceInfo> Create(const NeoPipelineDesc& Desc);

			Uint32 GetHashedKey(const std::string Key);
			PipelineWithSRB GetPipeline(Uint32 Key);
		private:
			std::vector<PipelineSwitch> mKeyChain;
			std::unordered_map<Uint32, PipelineWithSRB> mPipelineStates;
		};
	}
}