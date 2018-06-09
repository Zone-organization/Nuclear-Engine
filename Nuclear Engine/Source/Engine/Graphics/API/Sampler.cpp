#include <Engine/Graphics/API/Sampler.h>
#include <Engine\Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			Sampler::Sampler()
			{
			}

			Sampler::~Sampler()
			{
			}
			void Sampler::Create(Sampler *obj, const SamplerDesc& Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, Desc)

			}

			void Sampler::Delete(Sampler * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)

			}
			void Sampler::VSBind(Uint8 slot)
			{
				BASE_API_FUNC_CALL_ARGS(VSBind, slot)
			}

			void Sampler::PSBind(Uint8 slot)
			{
				BASE_API_FUNC_CALL_ARGS(PSBind, slot)

			}
			void Sampler::GSBind(Uint8 slot)
			{
				BASE_API_FUNC_CALL_ARGS(GSBind, slot)
			}
		}
	}
}