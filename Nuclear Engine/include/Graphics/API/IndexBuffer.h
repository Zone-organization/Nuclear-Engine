#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics/API/OpenGL\GLIndexBuffer.h>
#include <Graphics/API/DirectX\DX11IndexBuffer.h>
namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {

			class NEAPI IndexBuffer
			{
			public:
				IndexBuffer();
				~IndexBuffer();

				static void Create(IndexBuffer *buffer, void* indices, unsigned int count);
				static void Delete(IndexBuffer *buffer);

				void Bind();
			private:
				OpenGL::GLIndexBuffer GLObject;
				DirectX::DX11IndexBuffer DXObject;
			};
		}
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLIndexBuffer.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef OpenGL::GLIndexBuffer IndexBuffer;
		}
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <Graphics/API/DirectX\DX11IndexBuffer.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			typedef DirectX::DX11IndexBuffer IndexBuffer;
		}
	}
}
#endif
#endif