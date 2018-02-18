#pragma once
#include <API\Texture_Types.h>
#include <API\DirectX\DX11Common.h>

namespace NuclearEngine
{
	namespace API 
	{
		namespace DirectX
		{
			inline DXGI_FORMAT DXTypeMap(Format format)
			{
				switch (format)
				{
				case Format::R8_UNORM: return DXGI_FORMAT_R8_UNORM;
				case Format::R8G8_UNORM: return DXGI_FORMAT_R8G8_UNORM;
				case Format::R8G8B8_UNORM:
					Log.Warning("[DirectX] R8G8B8_UNORM Format isn't supported in DX Backend, setting to R8G8B8A8_UNORM by default.\n");
					return DXGI_FORMAT_R8G8B8A8_UNORM;
				case Format::R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
				case Format::R16_FLOAT: return DXGI_FORMAT_R16_FLOAT;
				case Format::R16G16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;
				case Format::R16G16B16_FLOAT:
					Log.Warning("[DirectX] R16G16B16_FLOAT Format isn't supported in DX Backend,  setting to R16G16B16A16_FLOAT by default.\n");
					return DXGI_FORMAT_R16G16B16A16_FLOAT;
				case Format::R16G16B16A16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
				case Format::R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
				case Format::R32G32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
				case Format::R32G32B32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
				case Format::R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
				default: return DXGI_FORMAT_R8G8B8A8_UNORM;
				}
			}
			inline D3D11_TEXTURE_ADDRESS_MODE DXTypeMap(TextureWrap textureWrap)
			{
				switch (textureWrap)
				{
				case TextureWrap::Repeat: return D3D11_TEXTURE_ADDRESS_WRAP;
				case TextureWrap::MirroredRepeat: return D3D11_TEXTURE_ADDRESS_MIRROR;
				case TextureWrap::ClampToEdge: return D3D11_TEXTURE_ADDRESS_CLAMP;
				case TextureWrap::ClampToBorder: return D3D11_TEXTURE_ADDRESS_BORDER;
				default: return D3D11_TEXTURE_ADDRESS_WRAP;
				}
			}
			inline D3D11_FILTER DXTypeMap(TextureFilter TextureFilter)
			{
				switch (TextureFilter)
				{
				case TextureFilter::Point: return D3D11_FILTER_MIN_MAG_MIP_POINT;
				case TextureFilter::Bilinear: return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				case TextureFilter::Trilinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

				default: return D3D11_FILTER_MIN_MAG_MIP_POINT;
				}
			}
		}
	}
}