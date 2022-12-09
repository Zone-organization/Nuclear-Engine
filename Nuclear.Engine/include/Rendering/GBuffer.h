#pragma once
#include <Graphics/RenderTarget.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{
		struct GBufferDesc {
			std::string mGbufferName = "GBuffer";
			Math::Vector2ui mDimensions = Math::Vector2ui(800,600);
			std::vector<Graphics::RenderTargetDesc> mRTDescs;
		};

		//Dynamic GBuffer Creation
		class NEAPI GBuffer
		{
		public:
			GBuffer();
			~GBuffer();

			void Initialize(const GBufferDesc& desc);

			void Bake(const Math::Vector2ui& dimensions);

			void Resize(const Math::Vector2ui& dimensions);

			bool isInitialized() const;

			Math::Vector2ui GetDimensions() const;

			std::vector<Graphics::RenderTarget> mRenderTargets;
		private:
			GBufferDesc mDesc;
			bool mBaked = false;
			bool mInitialized = false;
		};
	}
}