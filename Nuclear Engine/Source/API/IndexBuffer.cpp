#include "API\IndexBuffer.h"
#include <Core\Context.h>
#include <NuclearRendererBase\NRBIndexBuffer.h>

namespace NuclearEngine {
	namespace API {
		IndexBuffer::IndexBuffer()
		{
		}
		IndexBuffer::IndexBuffer(void* indices, unsigned int count)
		{
			buf = Core::Context::ConstructIndexBuffer(buf);

			buf->Create(indices, count);
		}
		IndexBuffer::~IndexBuffer()
		{
			if (buf != nullptr)
			{
				buf->Delete();
				delete buf;
				buf = nullptr;
			}
		}

		void IndexBuffer::Bind()
		{
			buf->Bind();
		}
		void IndexBuffer::Unbind()
		{
			buf->Unbind();
		}

		NuclearRenderer::NRBIndexBuffer * IndexBuffer::GetBase()
		{
			return buf;
		}

	}
}