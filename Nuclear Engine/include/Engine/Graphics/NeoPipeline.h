#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <unordered_map>
#include <Core\Path.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct PipelineKey
		{
			std::string KeyName;
			std::string KeyPrefix = "NE_ENABLE_";
			bool CreateSRB = true;
			bool InitSRBStaticResources = true;

			//AUTO FILLED ON CREATING NEOPIPELINE
			Uint32 mHashedName = 0;
		};

		typedef std::vector<PipelineKey> KeyChain;

		struct NeoPipelineDesc
		{
			std::string mName;

			Core::Path mVShaderPath;
			Core::Path mPShaderPath;

			std::vector<KeyChain> mKeyChain;
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
			std::vector<KeyChain> Create(const NeoPipelineDesc& Desc);

			Uint32 GetHashedKey(const std::string Key);
			PipelineWithSRB GetPipeline(Uint32 Key);
		private:
			std::vector<KeyChain> mKeyChain;
			std::unordered_map<Uint32, PipelineWithSRB> mPipelineStates;
		};
	}
}