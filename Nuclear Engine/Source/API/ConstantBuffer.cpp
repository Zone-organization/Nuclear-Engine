#include <API\ConstantBuffer.h>
#include <Core\Context.h>
#include <NuclearRendererBase\NRBConstantBuffer.h>

namespace NuclearEngine {
	namespace API {
		ConstantBuffer::ConstantBuffer()
		{

		}
		ConstantBuffer::ConstantBuffer(const char *_nameinshaders, unsigned int size)
		{
			buf = Core::Context::ConstructConstantBuffer(buf);

			//Align the buffer to be a multiple of 16
			int remainder = size % 16;
			if (remainder != 0)
			{
				Log->Warning("[ConstantBuffer] The size of buffer isn't a multiple of 16 which can cause many unexpected problems! \n");
			}

			buf->Create(_nameinshaders, size);
		}
		ConstantBuffer::~ConstantBuffer()
		{
			if (buf != nullptr)
			{
				buf->Delete();
				delete buf;
				buf = nullptr;
			}
		}
		void ConstantBuffer::Update(const void* data, unsigned int size)
		{
			buf->Update(data, size);
		}
		NuclearRenderer::NRBConstantBuffer * ConstantBuffer::GetBase()
		{
			return buf;
		}
	}
}