#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL/GLIndexBuffer.h>
#include <Engine/Graphics/API/DirectX/DX11IndexBuffer.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct IndexBufferDesc;

			class NEAPI IndexBuffer //<OpenGL::GLIndexBuffer, DirectX::DX11IndexBuffer>
			{
			public:
				IndexBuffer();
				~IndexBuffer();

				static void Create(IndexBuffer *buffer, const IndexBufferDesc& desc);
				static void Delete(IndexBuffer *buffer);

				void Bind();
				void Update(const void* data, unsigned int size);
				
				BASE_API(IndexBuffer)

			};
		}
	}
}