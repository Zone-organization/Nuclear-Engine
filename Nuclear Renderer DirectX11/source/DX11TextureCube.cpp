#include "NuclearRendererDX11\DX11TextureCube.h"
#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearCommon\Common_API_Types.h>
using namespace NuclearEngine;

namespace NuclearRenderer {
	DXGI_FORMAT GetDXTextureFormat(TextureFormat format);
	D3D11_TEXTURE_ADDRESS_MODE GetDXTextureWrap(TextureWrap textureWrap);
	D3D11_FILTER GetDXTextureFilter(TextureFilter textureFilter);

	DX11TextureCube::DX11TextureCube()
	{
	}
	bool DX11TextureCube::Create(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc)
	{
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));


		texDesc.Format = GetDXTextureFormat(Desc.Format);
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.CPUAccessFlags = 0;
		texDesc.ArraySize = 6;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		if (Desc.Filter == TextureFilter::Point2D || Desc.Filter == TextureFilter::Linear2D)
		{
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			texDesc.MipLevels = 1;
		}
		else
		{
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			texDesc.MipLevels = 10;
		}

		D3D11_SUBRESOURCE_DATA subData[6];
		for (unsigned int i = 0; i < data.size(); i++)
		{
			texDesc.Width = data[i].width;
			texDesc.Height = data[i].height;
			subData[i].pSysMem = data[i].databuf;
			subData[i].SysMemPitch = data[i].width * 4;
			subData[i].SysMemSlicePitch = 0;
		}

		if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, &subData[0], &textureID)))
		{
			return false;
		}
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;

		DX11Context::GetDevice()->CreateShaderResourceView(textureID, &srvDesc, &resourceView);

		return true;
	}
	void DX11TextureCube::Delete()
	{
		textureID->Release();
		resourceView->Release();
		samplerState->Release();
	}
	void DX11TextureCube::Bind(unsigned int index)
	{
	}
	void DX11TextureCube::Bind(const char * samplerName, NRBShader * shader, unsigned int index)
	{
	}
	void DX11TextureCube::Unbind()
	{
	}
	void DX11TextureCube::Unbind(unsigned int index)
	{
	}
	unsigned int DX11TextureCube::GLGetTextureID()
	{
		return 0;
	}

	DXGI_FORMAT GetDXTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8: return DXGI_FORMAT_R8_UNORM;
		case TextureFormat::R8G8: return DXGI_FORMAT_R8G8_UNORM;
		case TextureFormat::R8G8B8: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case TextureFormat::R8G8B8A8: return DXGI_FORMAT_R8G8B8A8_UNORM;
		default: return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
	}
	D3D11_TEXTURE_ADDRESS_MODE GetDXTextureWrap(TextureWrap textureWrap)
	{
		switch (textureWrap)
		{
		case TextureWrap::Repeat: return D3D11_TEXTURE_ADDRESS_WRAP;
		case TextureWrap::MirroredReapeat: return D3D11_TEXTURE_ADDRESS_MIRROR;
		case TextureWrap::ClampToEdge: return D3D11_TEXTURE_ADDRESS_CLAMP;
		case TextureWrap::ClampToBorder: return D3D11_TEXTURE_ADDRESS_BORDER;
		default: return D3D11_TEXTURE_ADDRESS_WRAP;
		}
	}
	D3D11_FILTER GetDXTextureFilter(TextureFilter textureFilter)
	{
		switch (textureFilter)
		{
		case TextureFilter::Point: return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case TextureFilter::Bilinear: return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case TextureFilter::Trilinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		default: return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}

	}
}