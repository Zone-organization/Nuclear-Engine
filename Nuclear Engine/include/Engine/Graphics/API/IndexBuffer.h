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

			class NEAPI IndexBuffer //<OpenGL::GLIndexBuffer, DirectX::DX11IndexBuffer>
			{
			public:
				IndexBuffer();
				~IndexBuffer();

				static void Create(IndexBuffer *buffer, void* indices, unsigned int count);
				static void Delete(IndexBuffer *buffer);

				void Bind();
				void Update(const void* data, unsigned int size);
				
				BASE_API(IndexBuffer)

			};
		}
	}
}