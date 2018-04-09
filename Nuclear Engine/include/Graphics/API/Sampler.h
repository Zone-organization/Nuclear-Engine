#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLSampler.h>
#include <Graphics/API/DirectX\DX11Sampler.h>

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
				void VSBind(unsigned int slot);
				void GSBind(unsigned int slot);
				void PSBind(unsigned int slot);

				BASE_API(Sampler)
			};
		}
	}
}