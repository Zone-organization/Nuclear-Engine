#pragma once
#include <Base\NE_Common.h>
#include <LLGL/Buffer.h>
#include <LLGL/ResourceHeap.h>
#include <LLGL/GraphicsPipeline.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct RenderSubObject
		{
			LLGL::Buffer*			mVertexBuffer = nullptr;
			LLGL::Buffer*			mIndexBuffer = nullptr;
			LLGL::ResourceHeap*		mResourceHeaps = nullptr;
			LLGL::GraphicsPipeline*	mPipeline = nullptr;

			Uint32 mResourceHeapStartSet = 0;
			Uint32 mIndicesCount = 0;
			Uint32 mIndicesOffset = 0;
		};

		struct RenderObject
		{
			LLGL::GraphicsPipeline*	mPipeline = nullptr;
			std::vector<RenderSubObject> mSubObjects;
		};
	}
}
