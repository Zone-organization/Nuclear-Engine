#pragma once
#include <Engine/Graphics/RenderTarget.h>
#include <vector>
namespace Nuclear
{
	namespace Rendering
	{
		struct GBufferDesc {
			std::string mGbufferName = "GBuffer";
			Uint32 RTWidth = 800;
			Uint32 RTHeight = 600;
			std::vector<Graphics::RenderTargetDesc> mRTDescs;
		};

		//Dynamic GBuffer Creation
		class NEAPI GBuffer
		{
		public:

			void Initialize(const GBufferDesc& desc);

			void Bake(Uint32 width, Uint32 height);

			void Resize(Uint32 width, Uint32 height);

			std::vector<Graphics::RenderTarget> mRenderTargets;
		private:
			GBufferDesc mDesc;
			bool mBaked = false;
		};
	}
}