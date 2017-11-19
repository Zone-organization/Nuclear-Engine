#pragma once
#include <NuclearRendererBase\NRBTextureCube.h>
#include <NuclearRendererDX11\DX11Common.h>

namespace NuclearRenderer {
	class NRDX11API DX11TextureCube : public NRBTextureCube
	{
	public:
		DX11TextureCube();

		bool Create(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc) override;
		void Delete() override;
		void Bind(unsigned int index) override;
		void Bind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void Unbind() override;
		void Unbind(unsigned int index) override;

		unsigned int GLGetTextureID() override;

	private:
		ID3D11Texture2D* textureID;
		ID3D11ShaderResourceView* resourceView;
		ID3D11SamplerState* samplerState;
	};
}