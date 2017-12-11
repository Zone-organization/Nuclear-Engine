#include <API\UniformBuffer.h>
#include <Core\Context.h>
#include <NuclearRendererBase\NRBUniformBuffer.h>

namespace NuclearEngine {
	namespace API {
		UniformBuffer::UniformBuffer()
		{

		}
		UniformBuffer::UniformBuffer(const char *_nameinshaders, unsigned int size)
		{
			buf = Core::Context::ConstructUniformBuffer(buf);

			//Align the buffer to be a multiple of 16
			int remainder = size % 16;
			if (remainder != 0)
			{
				Log->Warning("[UniformBuffer] The size of buffer isn't a multiple of 16 which can cause many unexpected problems! \n");
			}

			buf->Create(_nameinshaders, size);
		}
		UniformBuffer::~UniformBuffer()
		{
			if (buf != nullptr)
			{
				buf->Delete();
				delete buf;
				buf = nullptr;
			}
		}
		void UniformBuffer::Update(const void* data, unsigned int size)
		{
			buf->Update(data, size);
		}
		NuclearRenderer::NRBUniformBuffer * UniformBuffer::GetBase()
		{
			return buf;
		}
	}
}