#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLConstantBuffer.h>
#include <API\DirectX\DX11ConstantBuffer.h>
#include <API\VertexShader.h>
#include <API\PixelShader.h>

namespace NuclearEngine {
	namespace API {

		class NEAPI ConstantBuffer
		{
			friend class VertexShader;
			friend class PixelShader;
		public:
			ConstantBuffer();
			~ConstantBuffer();
			static void Create(ConstantBuffer* buffer, const char *_nameinshaders, unsigned int size);
			static void Delete(ConstantBuffer* buffer);

			void Update(const void* data, unsigned int size);
		private:
			OpenGL::GLConstantBuffer GLObject;
			DirectX::DX11ConstantBuffer DXObject;
		};
	}
}

#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLConstantBuffer.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLConstantBuffer ConstantBuffer;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11ConstantBuffer.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11ConstantBuffer ConstantBuffer;
	}
}
#endif
#endif