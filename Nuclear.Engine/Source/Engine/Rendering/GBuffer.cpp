#include "Engine/Rendering/GBuffer.h"

namespace Nuclear
{
	namespace Rendering
	{
		GBuffer::GBuffer()
		{
			mInitialized = false;
			mRenderTargets = std::vector<Graphics::RenderTarget>();
			mDesc = GBufferDesc();
		}
		GBuffer::~GBuffer()
		{
		}
		void GBuffer::Initialize(const GBufferDesc& desc)
		{
			mDesc = desc;
			mInitialized = true;
		}
		void GBuffer::Bake(Uint32 width, Uint32 height)
		{
			if (mInitialized)
			{
				if (!mBaked)
				{
					mRenderTargets.clear();
					for (auto& i : mDesc.mRTDescs)
					{
						Graphics::RenderTarget newrt;
						i.Width = width;
						i.Height = height;
						i.mType = mDesc.mGbufferName;
						newrt.Create(i);
						mRenderTargets.push_back(newrt);
					}
					mBaked = true;
				}
				else {
					Resize(width, height);
				}
			}
		}
		void GBuffer::Resize(Uint32 width, Uint32 height)
		{
			if (mInitialized)
			{
				for (auto& i : mRenderTargets)
				{
					i.Resize(width, height);
				}
			}
		}

		bool GBuffer::isInitialized() const
		{
			return mInitialized;
		}

	}
}