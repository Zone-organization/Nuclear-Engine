#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLConstantBuffer.h>
#include <Graphics/API/DirectX\DX11ConstantBuffer.h>
#include <Graphics/API/VertexShader.h>
#include <Graphics/API/PixelShader.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			class NEAPI ConstantBuffer //<OpenGL::GLConstantBuffer, DirectX::DX11ConstantBuffer>
			{
				friend class VertexShader;
				friend class PixelShader;
			public:
				ConstantBuffer();
				~ConstantBuffer();
				static void Create(ConstantBuffer* buffer, const char *_nameinshaders, unsigned int size);
				static void Delete(ConstantBuffer* buffer);

				void Update(const void* data, unsigned int size);
				BASE_API(ConstantBuffer)

			};
		}
	}
}