#include "Rendering/GBuffer.h"

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
		void GBuffer::Bake(const Math::Vector2ui& dimensions)
		{
			if (mInitialized)
			{
				if (!mBaked)
				{
					mRenderTargets.clear();
					for (auto& i : mDesc.mRTDescs)
					{
						Graphics::RenderTarget newrt;
						i.mDimensions = dimensions;
						i.mType = mDesc.mGbufferName;
						newrt.Create(i);
						mRenderTargets.push_back(newrt);
					}
					mBaked = true;
				}
				else {
					Resize(dimensions);
				}
			}
		}
		void GBuffer::Resize(const Math::Vector2ui& dimensions)
		{
			if (mInitialized)
			{
				mDesc.mDimensions = dimensions;
				for (auto& i : mRenderTargets)
				{
					i.Resize(dimensions);
				}
			}
		}

		bool GBuffer::isInitialized() const
		{
			return mInitialized;
		}

		Math::Vector2ui GBuffer::GetDimensions() const
		{
			return mDesc.mDimensions;
		}

	}
}