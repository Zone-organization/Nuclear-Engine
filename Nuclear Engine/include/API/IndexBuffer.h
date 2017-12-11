#pragma once
#include <NE_PrivateCommon.h>

namespace NuclearRenderer
{
	class NRBIndexBuffer;
}

namespace NuclearEngine {
	namespace API {

		class NEAPI IndexBuffer
		{
		public:
			IndexBuffer();
			IndexBuffer(void* indices, unsigned int count);
			~IndexBuffer();

			void Bind();
			void Unbind();

			NuclearRenderer::NRBIndexBuffer *GetBase();
		protected:
			NuclearRenderer::NRBIndexBuffer *buf;
		};
	}
}