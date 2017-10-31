#pragma once
#include <NE_PrivateCommon.h>
#include <NuclearCommon\Common_API_Types.h>

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
	
			NuclearRenderer::NRBUniformBuffer* GetBase();
		protected:
			NuclearRenderer::NRBUniformBuffer *buf;
		};
	}
}