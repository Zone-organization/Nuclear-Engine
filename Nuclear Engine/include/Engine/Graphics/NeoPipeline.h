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
		};

		struct NeoPipelineDesc
		{
			std::string mName;

			Core::Path mVShaderPath;
			Core::Path mPShaderPath;

			std::vector<PipelineKey> mKeys;
			std::vector<std::string> mConstDefines;

			bool SaveKeysInfo = DEBUG_TRUE_BOOL;

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

			bool Create(NeoPipelineDesc& Desc);

			Uint32 GetHashedKey(const std::string Key);
			PipelineWithSRB GetPipeline(Uint32 Key);

		private:
			std::vector<PipelineKey> mKeys;
			std::unordered_map<Uint32, PipelineWithSRB> mPipelineStates;
		};
	}
}