#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLPixelShader.h>
#include <API\DirectX\DX11PixelShader.h>

namespace NuclearEngine {
	namespace API {
		class ConstantBuffer;
		class NEAPI PixelShader
		{
			friend class Texture;
		public:
			PixelShader();
			~PixelShader();

			static void	Create(PixelShader* shader, BinaryShaderBlob* Desc);
			static void	Delete(PixelShader* shader);

			void SetConstantBuffer(ConstantBuffer* cbuffer);
			void Bind();

		private:
			OpenGL::GLPixelShader GLObject;
			DirectX::DX11PixelShader DXObject;
		};
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLPixelShader.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLPixelShader PixelShader;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11PixelShader.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11PixelShader PixelShader;
	}
}
#endif
#endif