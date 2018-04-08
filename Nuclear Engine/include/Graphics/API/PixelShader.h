#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLPixelShader.h>
#include <Graphics/API/DirectX\DX11PixelShader.h>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			class ConstantBuffer;
			class NEAPI PixelShader : public CBaseAPI<OpenGL::GLPixelShader, DirectX::DX11PixelShader>
			{
				friend class Texture;
			public:
				PixelShader();
				~PixelShader();

				static void	Create(PixelShader* shader, BinaryShaderBlob* Desc);
				static void	Delete(PixelShader* shader);

				void SetConstantBuffer(ConstantBuffer* cbuffer);
				void Bind();
			};
		}
	}
}