#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <unordered_map>
#include <Core\Path.h>
#include <set>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct PipelineSwitch
		{
			PipelineSwitch(std::string KeyName_, bool CreateSRB_ = true, bool InitSRBStaticResources_ = true)
				: KeyName(KeyName_), CreateSRB(CreateSRB_), InitSRBStaticResources(InitSRBStaticResources_)
			{

			}
			std::string KeyName;
			bool CreateSRB;
			bool InitSRBStaticResources;

			//AUTO FILLED ON CREATING NEOPIPELINE
			Uint32 mHashedName = 0;
		};

		struct PipelineInstanceInfo
		{
			std::set<std::string> Defines;
			Uint32 mHashKey = 0;
			bool CreateSRB = false;
			bool InitStaticResources = false;
		};

		struct NeoPipelineDesc
		{
			std::string mName;

			Core::Path mVShaderPath;
			Core::Path mPShaderPath;

			std::vector<PipelineSwitch> Switches;

			bool ZeroInstanceCreateSRB = true;
			bool ZeroInstanceInitSRBStaticResources = true;

			std::vector<std::string> mVSConstDefines;
			std::vector<std::string> mPSConstDefines;

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
			~NeoPipeline();

			std::vector<PipelineInstanceInfo> Create(const NeoPipelineDesc& Desc);

			Uint32 GetHashedKey(const std::string Key);
			PipelineWithSRB GetPipeline(Uint32 Key);
		private:
			std::vector<PipelineSwitch> mKeyChain;
			std::unordered_map<Uint32, PipelineWithSRB> mPipelineStates;
		};
	}
}