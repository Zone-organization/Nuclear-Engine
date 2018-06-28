#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLUnifiedShader.h>
#include <Engine/Graphics/API/DirectX\DX11UnifiedShader.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			class ConstantBuffer;
			class VertexShader;
			class PixelShader;

			class NEAPI UnifiedShader
			{
				friend class Texture;
				friend class VertexBuffer;
			public:
				UnifiedShader();
				~UnifiedShader();

				static void	Create(UnifiedShader* shader, const BinaryShaderBlob& vshader, const BinaryShaderBlob& pshader);

				static void	Delete(UnifiedShader* shader);

				void SetConstantBuffer(ConstantBuffer* cbuffer, const ShaderType& type);
				void Bind();

				ShaderReflection VSReflection;
				ShaderReflection PSReflection;

				BASE_API(UnifiedShader)

			};
		}
	}
}