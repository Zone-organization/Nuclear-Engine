#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLVertexShader.h>
#include <API\DirectX\DX11VertexShader.h>

namespace NuclearEngine {
	namespace API {
		class ConstantBuffer;
		class NEAPI VertexShader
		{
			friend class Texture;
			friend class VertexBuffer;
		public:
			VertexShader();
			~VertexShader();

			static void	Create(VertexShader* shader, BinaryShaderBlob* Desc);
			static void	Delete(VertexShader* shader);

			void SetConstantBuffer(ConstantBuffer* cbuffer);
			void Bind();

		private:
			OpenGL::GLVertexShader GLObject;
			DirectX::DX11VertexShader DXObject;
		};
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLVertexShader.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLVertexShader VertexShader;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11VertexShader.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11VertexShader VertexShader;
	}
}
#endif
#endif