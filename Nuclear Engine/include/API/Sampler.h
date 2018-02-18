#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLSampler.h>
#include <API\DirectX\DX11Sampler.h>

namespace NuclearEngine {
	namespace API {
	
		class NEAPI Sampler
		{
		public:
			Sampler();
			~Sampler();

			static void Create(Sampler* texture, const SamplerDesc& Desc);
			static void Delete(Sampler* texture);

			void VSBind(unsigned int index);
			void GSBind(unsigned int index);
			void PSBind(unsigned int index);

		private:
			OpenGL::GLSampler GLObject;
			DirectX::DX11Sampler DXObject;
		};
	}
}

#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLSampler.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLSampler Sampler;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11Sampler.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11Sampler Sampler;
	}
}
#endif
#endif