#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBTexture2D.h>

namespace NuclearRenderer {
	class NRDX11API DX11Texture2D : public NRBTexture2D
	{
	public:
		DX11Texture2D();

		bool Create(NuclearEngine::Texture_Data TexData, NuclearEngine::Texture_Desc Desc) override;
		void Delete() override;
		void Bind(unsigned int index) override;
		void Bind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void Unbind() override;
		void Unbind(unsigned int index) override;

		unsigned int GLGetTextureID() override;

	protected:
		ID3D11Texture2D* textureID;
		ID3D11ShaderResourceView* resourceView;
		ID3D11SamplerState* samplerState;
	};
}