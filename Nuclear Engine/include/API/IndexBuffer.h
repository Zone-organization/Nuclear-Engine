#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDERER
#include <API\OpenGL\GLIndexBuffer.h>
#include <API\DirectX\DX11IndexBuffer.h>
namespace NuclearEngine {
	namespace API {

		class NEAPI IndexBuffer
		{
		public:
			IndexBuffer();
			~IndexBuffer();

			static void Create(IndexBuffer *buffer,void* indices, unsigned int count);

			void Bind();
		private:
			OpenGL::GLIndexBuffer GLObject;
			DirectX::DX11IndexBuffer DXObject;
		};
	}
}
#else
#ifdef NE_USE_OPENGL3_3
#include <API\OpenGL\GLIndexBuffer.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLIndexBuffer IndexBuffer;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11IndexBuffer.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11IndexBuffer IndexBuffer;
	}
}
#endif
#endif