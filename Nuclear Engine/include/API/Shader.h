#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLShader.h>
#include <API\DirectX\DX11Shader.h>

namespace NuclearEngine {
	namespace API {

		class ConstantBuffer;
		struct ShaderDesc;
		enum class ShaderType;
		class Texture;
		class VertexBuffer;

		class NEAPI Shader
		{
			friend class Texture;
			friend class VertexBuffer;

		public:		
			Shader();
			~Shader();

			static void	Create(Shader* shader, ShaderDesc* Desc);
			void SetConstantBuffer(ConstantBuffer* cbuffer,ShaderType type);
			void Bind();

		private:
			OpenGL::GLShader GLObject;
			DirectX::DX11Shader DXObject;
		};
	}
}
#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLShader.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLShader Shader;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11Shader.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11Shader Shader;
	}
}
#endif
#endif