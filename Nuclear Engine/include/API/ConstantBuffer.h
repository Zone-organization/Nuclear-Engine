#pragma once
#include <NE_PrivateCommon.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer {
	class NRBConstantBuffer;
}

namespace NuclearEngine {
	namespace API {

		class NEAPI ConstantBuffer
		{
		public:
			ConstantBuffer();
			ConstantBuffer(const char *_nameinshaders, unsigned int size);
			~ConstantBuffer();

			void Update(const void* data, unsigned int size);
	
			NuclearRenderer::NRBConstantBuffer* GetBase();
		protected:
			NuclearRenderer::NRBConstantBuffer *buf;
		};
	}
}