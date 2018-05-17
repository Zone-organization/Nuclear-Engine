#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLVertexShader.h>
#include <Engine/Graphics/API/DirectX\DX11VertexShader.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			class ConstantBuffer;
			class NEAPI VertexShader //<OpenGL::GLVertexShader, DirectX::DX11VertexShader>
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

				ShaderReflection Reflection;

				BASE_API(VertexShader)

			};
		}
	}
}