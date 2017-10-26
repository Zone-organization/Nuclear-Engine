#include "API\IndexBuffer.h"
#include <Core\Context.h>
#include <NuclearRendererBase\NRBIndexBuffer.h>

namespace NuclearEngine {
	namespace API {
		IndexBuffer::IndexBuffer(void* indices, unsigned int count)
		{
			buf = Core::Context::ConstructIndexBuffer(buf);

			buf->Create(indices, count);
		}
		IndexBuffer::~IndexBuffer()
		{
			buf->Delete();

			delete buf;
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