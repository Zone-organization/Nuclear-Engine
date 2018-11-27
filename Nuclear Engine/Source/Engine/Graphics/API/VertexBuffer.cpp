#include <Engine/Graphics/API/VertexBuffer.h>
#include <Engine\Graphics\API\Context.h>
#include <Engine/Graphics/API/InputLayout.h>
#include <Engine/Graphics/API/VertexShader.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			VertexBuffer::VertexBuffer()
			{
			}

			VertexBuffer::~VertexBuffer()
			{
			}

			void VertexBuffer::Create(VertexBuffer * obj, const VertexBufferDesc& desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, desc)
			}

			void VertexBuffer::Delete(VertexBuffer * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}

			void VertexBuffer::Update(const void * data, unsigned int size)
			{
				BASE_API_FUNC_CALL_ARGS(Update, data, size)
			}

			void VertexBuffer::SetInputLayout(InputLayout * layout, VertexShader * shader)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.SetInputLayout(layout, &shader->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.SetInputLayout(layout, &shader->DXObject);
				}
			}

			void VertexBuffer::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
			void * VertexBuffer::Map()
			{
				SPECIAL_BUFFER_API_MAP_FUNC_CALL();
			}
			void VertexBuffer::Unmap()
			{
				BASE_API_FUNC_CALL(Unmap)
			}
		}
	}
}