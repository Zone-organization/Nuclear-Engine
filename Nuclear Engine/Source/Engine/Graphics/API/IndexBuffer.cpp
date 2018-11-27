#include <Engine/Graphics/API/IndexBuffer.h>
#include <Engine\Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			
			IndexBuffer::IndexBuffer()
			{
			}

			IndexBuffer::~IndexBuffer()
			{

			}

			void IndexBuffer::Create(IndexBuffer* obj, const IndexBufferDesc& desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, desc)
			}

			void IndexBuffer::Delete(IndexBuffer * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}

			void IndexBuffer::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
			void IndexBuffer::Update(const void * data, unsigned int size)
			{
				BASE_API_FUNC_CALL_ARGS(Update, data, size)
			}
			void * IndexBuffer::Map()
			{
				SPECIAL_BUFFER_API_MAP_FUNC_CALL();
			}
			void IndexBuffer::Unmap()
			{
				BASE_API_FUNC_CALL(Unmap)
			}
		}
	}
}