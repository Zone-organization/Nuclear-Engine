#pragma once
#include <NE_Common.h>
#include <API\Shader.h>
#include <NuclearCommon\Common_API_Types.h>

namespace NuclearRenderer
{
	class NRBVertexBuffer;
}

namespace NuclearEngine {
	namespace API {
		class InputLayout;

		struct VertexBufferDesc {
			const void* data;
			unsigned int size;
			BufferGPUUsage usage = BufferGPUUsage::Default;
			BufferCPUAccess accessflag = BufferCPUAccess::Default;
		};

		class NEAPI VertexBuffer
		{
		public:
			VertexBuffer(const VertexBufferDesc& desc);
			~VertexBuffer();

			void Update(const void* data, unsigned int size);

			void SetInputLayout(InputLayout* layout, Shader* shader);

			void Bind();
			void Unbind();
		protected:
			NuclearRenderer::NRBVertexBuffer *buf;
		};
	}
}