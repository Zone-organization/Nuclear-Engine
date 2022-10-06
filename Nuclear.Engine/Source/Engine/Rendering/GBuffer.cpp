#include "Engine/Rendering/GBuffer.h"

namespace Nuclear
{
	namespace Rendering
	{
		void GBuffer::Initialize(const GBufferDesc& desc)
		{
			mDesc = desc;
		}
		void GBuffer::Bake(Uint32 width, Uint32 height)
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
		void GBuffer::Resize(Uint32 width, Uint32 height)
		{
			for (auto& i : mRenderTargets)
			{
				i.Resize(width, height);
			}
		}

	}
}