#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLPixelShader.h>
#include <Engine/Graphics/API/DirectX\DX11PixelShader.h>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			class ConstantBuffer;
			class NEAPI PixelShader //<OpenGL::GLPixelShader, DirectX::DX11PixelShader>
			{
				friend class Texture;
			public:
				PixelShader();
				~PixelShader();

				static void	Create(PixelShader* shader, const BinaryShaderBlob& Desc);
				static void	Delete(PixelShader* shader);

				void SetConstantBuffer(ConstantBuffer* cbuffer);
				void Bind();

				ShaderReflection Reflection;

				BASE_API(PixelShader)

			};
		}
	}
}