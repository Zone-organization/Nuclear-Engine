#pragma once
#include <NE_PrivateCommon.h>

namespace NuclearRenderer {
	class NRBUniformBuffer;
}

namespace NuclearEngine {
	namespace API {

		class NEAPI UniformBuffer
		{
		public:
			UniformBuffer(const char *_nameinshaders, unsigned int size);
			~UniformBuffer();

			void Update(void* data, unsigned int size);
			void Update(void * data, unsigned int offset, unsigned int size);

			unsigned int GetBindingIndex();
			const char * GetName();

			NuclearRenderer::NRBUniformBuffer* GetBase();
		protected:
			NuclearRenderer::NRBUniformBuffer *buf;
		};
	}
}