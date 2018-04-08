#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL/GLIndexBuffer.h>
#include <Graphics/API/DirectX/DX11IndexBuffer.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{

			class NEAPI IndexBuffer : public CBaseAPI<OpenGL::GLIndexBuffer, DirectX::DX11IndexBuffer>
			{
			public:
				IndexBuffer();
				~IndexBuffer();

				static void Create(IndexBuffer *buffer, void* indices, unsigned int count);
				static void Delete(IndexBuffer *buffer);

				void Bind();
			};
		}
	}
}