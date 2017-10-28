#include <API\UniformBuffer.h>
#include <Core\Context.h>
#include <NuclearRendererBase\NRBUniformBuffer.h>

namespace NuclearEngine {
	namespace API {

		static int ubosBindingindex = 0;

		UniformBuffer::UniformBuffer(const char *_nameinshaders, unsigned int size)
		{
			buf = Core::Context::ConstructUniformBuffer(buf);

			//Align the buffer to be a multiple of 16
			int remainder = size % 16;
			if (remainder != 0)
			{
				Log->Warning("[UniformBuffer] The size of buffer isn't a multiple of 16 which can cause many problems and errors! \n");
			}

			buf->Create(_nameinshaders, size, ubosBindingindex);

			//Increment the binding index
			ubosBindingindex++;
		}
		UniformBuffer::~UniformBuffer()
		{
			buf->Delete();

			delete buf;
		}

		void UniformBuffer::Update(void * data, unsigned int size)
		{
			buf->Update(data, 0, size);
		}

		void UniformBuffer::Update(void * data, unsigned int offset, unsigned int size)
		{
			buf->Update(data, offset, size);
		}

		unsigned int UniformBuffer::GetBindingIndex()
		{
			return buf->GetBindingIndex();
		}
		const char * UniformBuffer::GetName()
		{
			return buf->GetName();
		}
		NuclearRenderer::NRBUniformBuffer * UniformBuffer::GetBase()
		{
			return buf;
		}
	}
}