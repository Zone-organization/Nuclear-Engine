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

			void IndexBuffer::Create(IndexBuffer* obj, void * indices, unsigned int count)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, indices, count)
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
		}
	}
}