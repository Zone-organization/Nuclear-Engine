#pragma once

namespace NuclearPlatform
{
	class Window;
}

namespace NuclearRenderer
{
	class NRBVertexBuffer;
	class NRBIndexBuffer;
	class NRBUniformBuffer;
	class NRBRenderTarget;
	class NRBTexture2D;
	class NRBTextureCube;
	class NRBShader;

	namespace ContextDesc
	{
		static float MaxAnisotropicLevel = 0;
		static bool SupportAnisotropic = false;
	}

	class NRBContext
	{
	public:
		virtual bool Initialize() = 0;
		virtual void SetPrimitiveType(int primitivetype) = 0;
		virtual void ClearColor(float Red, float Green, float Blue, float Alpha) = 0;

		virtual void SwapBuffers() = 0;
		virtual void Shutdown() = 0;

		virtual void Draw(unsigned int count) = 0;
		virtual void DrawIndexed(unsigned int vertexCount) = 0;

		virtual void EnableDepthBuffer(bool state) = 0;
		virtual void EnableBlending(bool state) = 0;

		virtual void SetViewPort(int x, int y, int width, int height) = 0;

		virtual NRBVertexBuffer* ConstructVertexBuffer(NRBVertexBuffer* param) = 0;
		virtual NRBIndexBuffer* ConstructIndexBuffer(NRBIndexBuffer* param) = 0;
		virtual NRBUniformBuffer* ConstructUniformBuffer(NRBUniformBuffer* param) = 0;
		virtual NRBRenderTarget* ConstructRenderTarget(NRBRenderTarget* param) = 0;		
		virtual NRBTexture2D* ConstructTexture2D(NRBTexture2D* param) = 0;
		virtual NRBTextureCube* ConstructTextureCube(NRBTextureCube* param) = 0;
		virtual NRBShader* ConstructShader(NRBShader* param) = 0;
	};
}