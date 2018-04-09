#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLVertexShader.h>
#include <Graphics/API/DirectX\DX11VertexShader.h>

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
				BASE_API(VertexShader)

			};
		}
	}
}