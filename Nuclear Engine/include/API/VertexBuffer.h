#pragma once
#include <NE_Common.h>
#include <API\Shader.h>

namespace NuclearRenderer
{
	class NRBVertexBuffer;
}

namespace NuclearEngine {
	namespace API {
		class InputLayout;

		enum class NEAPI BufferUsage {
			Default = 0,
			Static = 1,
			Dynamic = 2
		};

		enum class NEAPI BufferAccess {
			Default = 0,
			ReadOnly = 1,
			WriteOnly = 2
		};

		struct VertexBufferDesc {
			void* data;
			unsigned int size;
			BufferUsage usage = BufferUsage::Default;
			BufferAccess accessflag = BufferAccess::Default;
		};

		class NEAPI VertexBuffer
		{
		public:
			VertexBuffer(const VertexBufferDesc& desc);
			~VertexBuffer();

			void Update(void* data, unsigned int size);

			void SetInputLayout(InputLayout* layout, Shader* shader);

			void Bind();
			void Unbind();
		protected:
			NuclearRenderer::NRBVertexBuffer *buf;
		};
	}
}