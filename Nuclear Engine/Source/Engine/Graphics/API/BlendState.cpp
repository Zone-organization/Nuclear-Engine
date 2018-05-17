#include <Engine/Graphics/API/BlendState.h>
#include <Engine\Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			BlendState::BlendState()
			{
			}

			BlendState::~BlendState()
			{

			}
			void BlendState::Create(BlendState* obj, const BlendStateDesc& type)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create,type)
			}
			void BlendState::Delete(BlendState * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}
			void BlendState::Bind(Graphics::Color blendfactor, unsigned int samplemask)
			{
				BASE_API_FUNC_CALL_ARGS(Bind, blendfactor, samplemask)
			}

		}
	}
}