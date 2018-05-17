#include <Engine/Graphics/API/RasterizerState.h>
#include <Engine\Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {

		namespace API {
			RasterizerState::RasterizerState()
			{
			}

			RasterizerState::~RasterizerState()
			{

			}
			void RasterizerState::Create(RasterizerState* obj, const RasterizerStateDesc& Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, Desc)
			}
			void RasterizerState::Delete(RasterizerState * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}
			void RasterizerState::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
		}
	}
}