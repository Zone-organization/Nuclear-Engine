#include <API\VertexBuffer.h>
#include <API\InputLayout.h>
#include <Core\Context.h>
#include <NuclearRendererBase\NRBVertexBuffer.h>

namespace NuclearEngine {
	namespace API {
		VertexBuffer::VertexBuffer()
		{
		}
		VertexBuffer::VertexBuffer(const VertexBufferDesc & desc)
		{
			buf = Core::Context::ConstructVertexBuffer(buf);
			buf->Create(desc.data, desc.size, desc.usage, desc.accessflag);
		}

		VertexBuffer::~VertexBuffer()
		{
			if (buf != nullptr)
			{
				buf->Delete();
				delete buf;
				buf = nullptr;
			}
		}

		void VertexBuffer::Update(const void * data, unsigned int size)
		{
			buf->Update(data, size);
		}

		void VertexBuffer::SetInputLayout(InputLayout* layout, Shader* shader)
		{
			buf->SetInputLayout(layout->GetBase(), shader->GetBase());
		}

		void VertexBuffer::Bind()
		{
			buf->Bind();
		}
		void VertexBuffer::Unbind()
		{
			buf->Unbind();
		}
		
	}
}