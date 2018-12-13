#include "Engine\Graphics\Renderer.h"

namespace NuclearEngine
{
	namespace Graphics
	{
		bool Renderer::Validate(RenderObject * object)
		{
			if (!object) return false;
			if (!object->mPipeline) return false;

			for (auto i : object->mSubObjects)
			{
				if (!i.mVertexBuffer) return false;
				if (!i.mIndexBuffer) return false;
				if (!i.mResourceHeaps) return false;
				if (i.mIndicesCount == 0) return false;
			}

			return true;
		}
		void Renderer::Render(RenderObject * object)
		{
			Graphics::Context::GetCommands()->SetGraphicsPipeline(*object->mPipeline);

			for (auto i : object->mSubObjects)
			{
				Graphics::Context::GetCommands()->SetVertexBuffer(*i.mVertexBuffer);
				Graphics::Context::GetCommands()->SetIndexBuffer(*i.mIndexBuffer);
				Graphics::Context::GetCommands()->SetGraphicsResourceHeap(*i.mResourceHeaps, i.mResourceHeapStartSet);
				Graphics::Context::GetCommands()->Draw(i.mIndicesCount, i.mIndicesOffset);
			}
		}
	}
}