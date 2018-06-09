#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLSampler.h>
#include <Engine/Graphics/API/DirectX\DX11Sampler.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			class NEAPI Sampler //<OpenGL::GLSampler, DirectX::DX11Sampler>
			{
			public:
				Sampler();
				~Sampler();

				static void Create(Sampler* texture, const SamplerDesc& Desc);
				static void Delete(Sampler* texture);

				//TODO: MOVE
				void VSBind(Uint8 slot);
				void GSBind(Uint8 slot);
				void PSBind(Uint8 slot);

				BASE_API(Sampler)
			};
		}
	}
}