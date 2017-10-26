#include <NuclearRendererDX11\DX11Context.h>
#include <NuclearRendererDX11\DX11Texture2D.h>
#include <NuclearCommon\Common_API_Types.h>
using namespace NuclearEngine; 
#include <vector>
namespace NuclearRenderer {

	DXGI_FORMAT GetDXTextureFormat(TextureFormat format);
	D3D11_TEXTURE_ADDRESS_MODE GetDXTextureWrap(TextureWrap textureWrap);
	D3D11_FILTER GetDXTextureFilter(TextureFilter textureFilter);

	DX11Texture2D::DX11Texture2D() :
		textureID(nullptr), resourceView(nullptr), samplerState(nullptr)
	{
	}

	bool DX11Texture2D::Create(Texture_Data Data, Texture_Desc Desc)
	{

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

		texDesc.Width = Data.width;
		texDesc.Height = Data.height;
		texDesc.Format = GetDXTextureFormat(Desc.Format);
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.CPUAccessFlags = 0;
		texDesc.ArraySize = 1;


		if (Desc.Filter == TextureFilter::Point2D || Desc.Filter == TextureFilter::Linear2D)
		{
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			texDesc.MiscFlags = 0;
			texDesc.MipLevels = 1;
		}
		else
		{
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			texDesc.MipLevels = 10;
		}

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));


		subData.pSysMem = Data.databuf;
		subData.SysMemPitch = Data.width * 4;
		subData.SysMemSlicePitch = 0;


		if (Desc.Filter == TextureFilter::Point2D || Desc.Filter == TextureFilter::Linear2D)
		{
			if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, &subData, &textureID)))
			{
				return false;
			}

			DX11Context::GetDevice()->CreateShaderResourceView(textureID, 0, &resourceView);
		}
		else
		{
			if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, nullptr, &textureID)))
			{
				return false;
			}
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = texDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 10;
			srvDesc.Texture2DArray.MostDetailedMip = 0;
			DX11Context::GetDevice()->CreateShaderResourceView(textureID, &srvDesc, &resourceView);
			DX11Context::GetContext()->UpdateSubresource(textureID, 0, 0, subData.pSysMem, subData.SysMemPitch, 0);
			DX11Context::GetContext()->GenerateMips(resourceView);

		}



		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		if (Desc.AnisoFilter == AnisotropicFilter::None)
		{
			samplerDesc.Filter = GetDXTextureFilter(Desc.Filter);
		}
		else
		{
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		}
		samplerDesc.AddressU = GetDXTextureWrap(Desc.Wrap);
		samplerDesc.AddressV = GetDXTextureWrap(Desc.Wrap);
		samplerDesc.AddressW = GetDXTextureWrap(Desc.Wrap);
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		switch (Desc.AnisoFilter)
		{
		case AnisotropicFilter::AnisotropicX2:
		{
			samplerDesc.MaxAnisotropy = 2;
		}
		case AnisotropicFilter::AnisotropicX4:
		{
			samplerDesc.MaxAnisotropy = 4;

		}
		case AnisotropicFilter::AnisotropicX8:
		{
			samplerDesc.MaxAnisotropy = 8;

		}
		case AnisotropicFilter::AnisotropicX16:
		{
			samplerDesc.MaxAnisotropy = 16;
		}

		default:
			samplerDesc.MaxAnisotropy = 0;
			break;
		}
		
		if (FAILED(DX11Context::GetDevice()->CreateSamplerState(&samplerDesc, &samplerState)))
		{
			return false;
		}

		return true;
	}
	void DX11Texture2D::Delete()
	{
		textureID->Release();
		resourceView->Release();
		samplerState->Release();
	}
	void DX11Texture2D::Bind(unsigned int index)
	{
		DX11Context::GetContext()->PSSetShaderResources(index, 1, &resourceView);
		DX11Context::GetContext()->PSSetSamplers(index, 1, &samplerState);
	}
	void DX11Texture2D::Bind(const char * samplerName, NRBShader * shader, unsigned int index)
	{
		DX11Context::GetContext()->PSSetShaderResources(index, 1, &resourceView);
		DX11Context::GetContext()->PSSetSamplers(index, 1, &samplerState);
	}

	void DX11Texture2D::Unbind()
	{
		return;
	}
	void DX11Texture2D::Unbind(unsigned int index)
	{
		return;
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