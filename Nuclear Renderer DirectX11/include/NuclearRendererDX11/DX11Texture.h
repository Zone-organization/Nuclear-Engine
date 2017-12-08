#pragma once
#include <NuclearRendererDX11\DX11Common.h>
#include <NuclearRendererBase/NRBTexture.h>

namespace NuclearRenderer {
	class NRDX11API DX11Texture : public NRBTexture
	{
	public:
		DX11Texture();

		void Create(NuclearEngine::Texture_Data TexData, NuclearEngine::Texture_Desc Desc) override;
		void Create(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc) override;
		void Delete() override;
		void VSBind(unsigned int index) override;
		void VSBind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void GSBind(unsigned int index) override;
		void GSBind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void PSBind(unsigned int index) override;
		void PSBind(const char *samplerName, NRBShader *shader, unsigned int index) override;

		unsigned int GLGetTextureID() override;

	protected:

		void Create1D(NuclearEngine::Texture_Data Data, NuclearEngine::Texture_Desc Desc);
		void Create2D(NuclearEngine::Texture_Data Data, NuclearEngine::Texture_Desc Desc);
		void Create3D(NuclearEngine::Texture_Data Data, NuclearEngine::Texture_Desc Desc);
		void CreateCube(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc);

		ComPtr<ID3D11Texture1D> tex1D;
		ComPtr<ID3D11Texture2D> tex2D;
		ComPtr<ID3D11Texture3D> tex3D;
		ComPtr<ID3D11ShaderResourceView> resourceView;
		ComPtr<ID3D11SamplerState> samplerState;
	};
}