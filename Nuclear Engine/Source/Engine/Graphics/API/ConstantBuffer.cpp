#include <Engine/Graphics/API/ConstantBuffer.h>
#include <Engine\Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			ConstantBuffer::ConstantBuffer()
			{

			}

			ConstantBuffer::~ConstantBuffer()
			{

			}
			void ConstantBuffer::Create(ConstantBuffer* obj, const char * name, unsigned int size)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, name,size)
			}
			void ConstantBuffer::Delete(ConstantBuffer * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}
			void ConstantBuffer::Update(const void* data, unsigned int size)
			{
				BASE_API_FUNC_CALL_ARGS(Update, data, size)
			}
		}
	}
}