#include <API\DirectX\DX11Texture.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11Context.h>
#include <API\DirectX\DX11Shader.h>
#include <API\DirectX\DX11VertexShader.h>
#include <API\DirectX\DX11PixelShader.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			DXGI_FORMAT GetDXFormat(Format format);
			D3D11_TEXTURE_ADDRESS_MODE GetDXTextureWrap(TextureWrap textureWrap);
			D3D11_FILTER GetDXTextureFilter(TextureFilter textureFilter);

			DX11Texture::DX11Texture() :
				tex1D(nullptr), tex2D(nullptr), tex3D(nullptr), resourceView(nullptr), samplerState(nullptr)
			{
			}

			DX11Texture::~DX11Texture()
			{
				tex1D = nullptr;
				tex2D = nullptr;
				tex3D = nullptr;
				resourceView = nullptr;
				samplerState = nullptr;
			}

			void DX11Texture::Create(DX11Texture* texture,const Texture_Data&Data, const Texture_Desc& Desc)
			{
				if (Desc.Type == TextureType::Texture1D)
				{
					return DX11Texture::Create1D(texture, Data, Desc);
				}
				else if (Desc.Type == TextureType::Texture2D)
				{
					return DX11Texture::Create2D(texture, Data, Desc);
				}
				else if (Desc.Type == TextureType::Texture3D)
				{
					return DX11Texture::Create2D(texture, Data, Desc);
				}
			}

			void DX11Texture::Create(DX11Texture* texture, const std::array<API::Texture_Data, 6>& data, const Texture_Desc& Desc)
			{
				return DX11Texture::CreateCube(texture, data, Desc);
			}

			void DX11Texture::Delete(DX11Texture * texture)
			{
				if (texture->tex1D != nullptr)
				{
					texture->tex1D->Release();
				}
				if (texture->tex2D != nullptr)
				{
					texture->tex2D->Release();
				}
				if (texture->tex3D != nullptr)
				{
					texture->tex3D->Release();
				}
				if (texture->resourceView != nullptr)
				{
					texture->resourceView->Release();
				}
				if (texture->samplerState != nullptr)
				{
					texture->samplerState->Release();
				}
				texture->tex1D = nullptr;
				texture->tex2D = nullptr;
				texture->tex3D = nullptr;
				texture->resourceView = nullptr;
				texture->samplerState = nullptr;
			}

			void DX11Texture::SetInShader(const char * samplerName, DX11VertexShader * shader, unsigned int index)
			{
				DX11Context::GetContext()->VSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->VSSetSamplers(index, 1, &samplerState);
			}

			void DX11Texture::SetInShader(const char * samplerName, DX11PixelShader * shader, unsigned int index)
			{
				DX11Context::GetContext()->PSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->PSSetSamplers(index, 1, &samplerState);
			}

			void DX11Texture::PSBind(unsigned int index)
			{
				DX11Context::GetContext()->PSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->PSSetSamplers(index, 1, &samplerState);
			}
			void DX11Texture::PSBind(const char * samplerName, DX11Shader * shader, unsigned int index)
			{
				DX11Context::GetContext()->PSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->PSSetSamplers(index, 1, &samplerState);
			}
			void DX11Texture::VSBind(unsigned int index)
			{
				DX11Context::GetContext()->VSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->VSSetSamplers(index, 1, &samplerState);
			}
			void DX11Texture::VSBind(const char * samplerName, DX11Shader * shader, unsigned int index)
			{
				DX11Context::GetContext()->VSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->VSSetSamplers(index, 1, &samplerState);
			}
			void DX11Texture::GSBind(const char * samplerName, DX11Shader * shader, unsigned int index)
			{
				DX11Context::GetContext()->GSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->GSSetSamplers(index, 1, &samplerState);
			}
			void DX11Texture::GSBind(unsigned int index)
			{
				DX11Context::GetContext()->GSSetShaderResources(index, 1, &resourceView);
				DX11Context::GetContext()->GSSetSamplers(index, 1, &samplerState);
			}
	
			void DX11Texture::Create1D(DX11Texture* result, const Texture_Data& Data, const Texture_Desc& Desc)
			{
				D3D11_TEXTURE1D_DESC texDesc;
				ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE1D_DESC));

				texDesc.Width = Data.Width;
				texDesc.Format = GetDXFormat(Desc.Format);
				texDesc.Usage = D3D11_USAGE_DEFAULT;
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


				subData.pSysMem = Data.Img_Data_Buf;
				subData.SysMemPitch = Data.Width * 4;
				subData.SysMemSlicePitch = 0;

				if (Desc.Filter == TextureFilter::Point2D || Desc.Filter == TextureFilter::Linear2D)
				{
					if (FAILED(DX11Context::GetDevice()->CreateTexture1D(&texDesc, &subData, &result->tex1D)))
					{
						Log.Error("[DirectX] Texture1D Creation Failed for Subdata filled Texture1D!\n");
					}

					DX11Context::GetDevice()->CreateShaderResourceView(result->tex1D, 0, &result->resourceView);
				}
				else
				{
					if (FAILED(DX11Context::GetDevice()->CreateTexture1D(&texDesc, nullptr, &result->tex1D)))
					{
						Log.Error("[DirectX] Texture1D Creation Failed for nullptr Subdata Texture1D!\n");

					}
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					srvDesc.Format = texDesc.Format;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
					srvDesc.Texture1D.MipLevels = 10;
					srvDesc.Texture1DArray.MostDetailedMip = 0;
					DX11Context::GetDevice()->CreateShaderResourceView(result->tex1D, &srvDesc, &result->resourceView);
					DX11Context::GetContext()->UpdateSubresource(result->tex1D, 0, 0, subData.pSysMem, subData.SysMemPitch, 0);
					DX11Context::GetContext()->GenerateMips(result->resourceView);

				}

				D3D11_SAMPLER_DESC samplerDesc;
				ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

				samplerDesc.Filter = GetDXTextureFilter(Desc.Filter);
				samplerDesc.AddressU = GetDXTextureWrap(Desc.Wrap);
				samplerDesc.AddressV = GetDXTextureWrap(Desc.Wrap);
				samplerDesc.AddressW = GetDXTextureWrap(Desc.Wrap);
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
				samplerDesc.MaxAnisotropy = 0;


				if (FAILED(DX11Context::GetDevice()->CreateSamplerState(&samplerDesc, &result->samplerState)))
				{
					Log.Error("[DirectX] SamplerState Creation Failed for Texture1D!\n");
				}
			}
			void DX11Texture::Create2D(DX11Texture* result,const Texture_Data& Data, const Texture_Desc& Desc)
			{
				D3D11_TEXTURE2D_DESC texDesc;
				ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

				texDesc.Width = Data.Width;
				texDesc.Height = Data.Height;
				texDesc.Format = GetDXFormat(Desc.Format);
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
					texDesc.MipLevels = 0;
				}

				//Check if null texture
				if (Data.Width == 0 && Data.Height == 0 && Data.Img_Data_Buf == NULL)
				{
					if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, nullptr, &result->tex2D)))
					{
						Log.Error("[DirectX] Texture2D Creation Failed for Null-ed Texture2D!\n");
						return;
					}
					DX11Context::GetDevice()->CreateShaderResourceView(result->tex2D, 0, &result->resourceView);

					D3D11_SAMPLER_DESC samplerDesc;
					ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

					
					samplerDesc.Filter = GetDXTextureFilter(Desc.Filter);
					
				
					samplerDesc.AddressU = GetDXTextureWrap(Desc.Wrap);
					samplerDesc.AddressV = GetDXTextureWrap(Desc.Wrap);
					samplerDesc.AddressW = GetDXTextureWrap(Desc.Wrap);
					samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
					samplerDesc.MinLOD = 0;
					samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;					
					samplerDesc.MaxAnisotropy = 0;				

					if (FAILED(DX11Context::GetDevice()->CreateSamplerState(&samplerDesc, &result->samplerState)))
					{
						Log.Error("[DirectX] SamplerState Creation Failed for Null-ed Texture2D!\n");
						return;
					}
					return;
				}
				D3D11_SUBRESOURCE_DATA subData;
				ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

				subData.pSysMem = Data.Img_Data_Buf;
				subData.SysMemPitch = Data.Width * 4;
				subData.SysMemSlicePitch = 0;

				if (Desc.Filter == TextureFilter::Point2D || Desc.Filter == TextureFilter::Linear2D)
				{
					if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, &subData, &result->tex2D)))
					{
						Log.Error("[DirectX] Texture2D Creation Failed for Subdata filled Texture2D!\n");
						return;
					}

					DX11Context::GetDevice()->CreateShaderResourceView(result->tex2D, 0, &result->resourceView);
				}
				else
				{
					if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, nullptr, &result->tex2D)))
					{
						Log.Error("[DirectX] Texture2D Creation Failed for nullptr Subdata Texture2D!\n");
						return;

					}
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					srvDesc.Format = texDesc.Format;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = -1;
					srvDesc.Texture2DArray.MostDetailedMip = 0;
					DX11Context::GetDevice()->CreateShaderResourceView(result->tex2D, &srvDesc, &result->resourceView);
					DX11Context::GetContext()->UpdateSubresource(result->tex2D, 0, 0, subData.pSysMem, subData.SysMemPitch, 0);
					DX11Context::GetContext()->GenerateMips(result->resourceView);

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

				if (FAILED(DX11Context::GetDevice()->CreateSamplerState(&samplerDesc, &result->samplerState)))
				{
					Log.Error("[DirectX] SamplerState Creation Failed for Texture2D!\n");
					return;

				}
				return;
			}
			void DX11Texture::Create3D(DX11Texture* result,const Texture_Data& Data, const Texture_Desc& Desc)
			{
				//TODO
				return;
			}
			void DX11Texture::CreateCube(DX11Texture* result, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc)
			{
				D3D11_TEXTURE2D_DESC texDesc;
				ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

				texDesc.Format = GetDXFormat(Desc.Format);
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
					texDesc.MipLevels = 0;
				}

				D3D11_SUBRESOURCE_DATA subData[6];
				for (unsigned int i = 0; i < data.size(); i++)
				{
					texDesc.Width = data[i].Width;
					texDesc.Height = data[i].Height;
					subData[i].pSysMem = data[i].Img_Data_Buf;
					subData[i].SysMemPitch = data[i].Width * 4;
					subData[i].SysMemSlicePitch = 0;
				}

				if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, &subData[0], &result->tex2D)))
				{
					Log.Error("[DirectX] TextureCube Creation Failed!\n");
				}
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.Format = texDesc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
				srvDesc.TextureCube.MostDetailedMip = 0;

				DX11Context::GetDevice()->CreateShaderResourceView(result->tex2D, &srvDesc, &result->resourceView);
			}
			DXGI_FORMAT GetDXFormat(Format format)
			{
				switch (format)
				{
				case Format::R8: return DXGI_FORMAT_R8_UNORM;
				case Format::R8G8: return DXGI_FORMAT_R8G8_UNORM;
				case Format::R8G8B8: return DXGI_FORMAT_R8G8B8A8_UNORM;
				case Format::R8G8B8A8: return DXGI_FORMAT_R8G8B8A8_UNORM;
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
	}
}
#endif