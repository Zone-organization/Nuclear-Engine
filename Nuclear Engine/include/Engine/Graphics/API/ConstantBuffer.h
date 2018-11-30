#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLConstantBuffer.h>
#include <Engine/Graphics/API/DirectX\DX11ConstantBuffer.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			class VertexShader;
			class PixelShader;

			class NEAPI ConstantBuffer //<OpenGL::GLConstantBuffer, DirectX::DX11ConstantBuffer>
			{
				friend class VertexShader;
				friend class PixelShader;
			public:
				ConstantBuffer();
				~ConstantBuffer();
				static void Create(ConstantBuffer* buffer, const char *_nameinshaders, unsigned int size);
				static void Delete(ConstantBuffer* buffer);

				void VSBind(Uint8 slot);
				void GSBind(Uint8 slot);
				void PSBind(Uint8 slot);

				void Update(const void* data, unsigned int size);
				BASE_API(ConstantBuffer)

			};
		}
	}
}