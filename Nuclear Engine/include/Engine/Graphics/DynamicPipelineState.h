#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <unordered_map>
#include <Core\Path.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		//struct Key
		//{
		//	std::string MainKey;
		//};

		struct DynamicPipelineStateDesc
		{
			std::string mName;

			Core::Path mVShaderPath;
			Core::Path mPShaderPath;

			std::vector<std::string> mKeys;
			std::vector<std::string> mConstDefines;

			GraphicsPipelineDesc GraphicsPipeline;
		};

		class NEAPI DynamicPipelineState
		{
		public:

			bool Create(DynamicPipelineStateDesc& Desc);

			Uint32 GetHashedKey(const std::string Key);
			RefCntAutoPtr<IPipelineState> GetPipeline(Uint32 Key);
		private:
			std::unordered_map<Uint32, RefCntAutoPtr<IPipelineState>> mPipelineStates;
		};
	}
}