#include <NuclearRendererBase\NRBContext.h>
#include <NuclearRendererDX11\DX11Common.h>

namespace NuclearRenderer
{
	class NRDX11API DX11Context : public NRBContext
	{
	public:
		bool Initialize() override;
		void SetPrimitiveType(int primitivetype) override;
		void ClearColor(float Red, float Green, float Blue, float Alpha) override;

		void SwapBuffers() override;
		void Shutdown() override;

		void Draw(unsigned int count) override;
		void DrawIndexed(unsigned int vertexCount) override;

		void EnableDepthBuffer(bool state) override;
		void EnableBlending(bool state) override;

		void SetViewPort(int x, int y, int width, int height) override;

		NRBVertexBuffer* ConstructVertexBuffer(NRBVertexBuffer* param) override;
		NRBIndexBuffer* ConstructIndexBuffer(NRBIndexBuffer* param) override;
		NRBUniformBuffer* ConstructUniformBuffer(NRBUniformBuffer* param) override;
		NRBRenderTarget* ConstructRenderTarget(NRBRenderTarget* param) override;
		NRBTexture2D* ConstructTexture2D(NRBTexture2D* param) override;
		NRBTextureCube* ConstructTextureCube(NRBTextureCube* param) override;
		NRBShader* ConstructShader(NRBShader* param) override;

		// Implementation Specific
		static ID3D11Device1* GetDevice();
		static ID3D11DeviceContext1* GetContext();
	};
}