#include <Graphics/API/DepthStencilState.h>
#include <Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {

		namespace API {
			DepthStencilState::DepthStencilState()
			{
			}

			DepthStencilState::~DepthStencilState()
			{

			}
			void DepthStencilState::Create(DepthStencilState* obj, const DepthStencilStateDesc& type)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, type)

			}
			void DepthStencilState::Delete(DepthStencilState * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)

			}
			void DepthStencilState::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
		}
	}
}