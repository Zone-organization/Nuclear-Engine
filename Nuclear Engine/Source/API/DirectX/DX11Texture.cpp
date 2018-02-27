#include <API\DirectX\DX11Texture.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11Context.h>
#include <API\DirectX\DX11Types.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			DX11Texture::DX11Texture() :
				tex1D(nullptr), tex2D(nullptr), tex3D(nullptr), resourceView(nullptr)
			{
			}

			DX11Texture::~DX11Texture()
			{
				tex1D = nullptr;
				tex2D = nullptr;
				tex3D = nullptr;
				resourceView = nullptr;
			}

			void DX11Texture::Create(DX11Texture* texture,const Texture_Data&Data, const Texture_Desc& Desc)
			{
				texture->desc = Desc;
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
				texture->desc = Desc;
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

				texture->tex1D = nullptr;
				texture->tex2D = nullptr;
				texture->tex3D = nullptr;
				texture->resourceView = nullptr;
			}

			void DX11Texture::PSBind(unsigned int slot)
			{
				DX11Context::GetContext()->PSSetShaderResources(slot, 1, &resourceView);
			}
		
			void DX11Texture::VSBind(unsigned int slot)
			{
				DX11Context::GetContext()->VSSetShaderResources(slot, 1, &resourceView);
			}
	
			void DX11Texture::GSBind(unsigned int slot)
			{
				DX11Context::GetContext()->GSSetShaderResources(slot, 1, &resourceView);
			}
	
			Texture_Desc DX11Texture::GetTextureDesc()
			{
				return desc;
			}

			void DX11Texture::Create1D(DX11Texture* result, const Texture_Data& Data, const Texture_Desc& Desc)
			{
				D3D11_TEXTURE1D_DESC texDesc;
				ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE1D_DESC));

				texDesc.Width = Data.Width;
				texDesc.Format = DXTypeMap(Desc.Format);
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.CPUAccessFlags = 0;
				texDesc.ArraySize = 1;

				if (Desc.GenerateMipMaps == false)
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

				if (Desc.GenerateMipMaps == false)
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
					srvDesc.Texture1D.MipLevels = -1;
					srvDesc.Texture1DArray.MostDetailedMip = 0;
					DX11Context::GetDevice()->CreateShaderResourceView(result->tex1D, &srvDesc, &result->resourceView);
					DX11Context::GetContext()->UpdateSubresource(result->tex1D, 0, 0, subData.pSysMem, subData.SysMemPitch, 0);
					DX11Context::GetContext()->GenerateMips(result->resourceView);

				}
			}
			void DX11Texture::Create2D(DX11Texture* result,const Texture_Data& Data, const Texture_Desc& Desc)
			{
				D3D11_TEXTURE2D_DESC texDesc;
				ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

				texDesc.Width = Data.Width;
				texDesc.Height = Data.Height;
				texDesc.Format = DXTypeMap(Desc.Format);
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.SampleDesc.Count = 1;
				texDesc.SampleDesc.Quality = 0;
				texDesc.CPUAccessFlags = 0;
				texDesc.ArraySize = 1;
				texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

				if (Desc.GenerateMipMaps == false)
				{
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
				if (Data.Img_Data_Buf == NULL)
				{
					if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, NULL, &result->tex2D)))
					{
						Log.Error("[DirectX] Texture2D Creation Failed for Null-ed Texture2D!\n");
						return;
					}
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					srvDesc.Format = texDesc.Format;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MostDetailedMip = 0;
					srvDesc.Texture2D.MipLevels = 1;

					DX11Context::GetDevice()->CreateShaderResourceView(result->tex2D, &srvDesc, &result->resourceView);

					return;
				}
				D3D11_SUBRESOURCE_DATA subData;
				ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

				subData.pSysMem = Data.Img_Data_Buf;
				subData.SysMemPitch = Data.Width * 4;
				subData.SysMemSlicePitch = 0;

				if (Desc.GenerateMipMaps == false)
				{
					if (FAILED(DX11Context::GetDevice()->CreateTexture2D(&texDesc, &subData, &result->tex2D)))
					{
						Log.Error("[DirectX] Texture2D Creation Failed for Subdata filled Texture2D!\n");
						return;
					}
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					srvDesc.Format = texDesc.Format;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MostDetailedMip = 0;
					srvDesc.Texture2D.MipLevels = 1;

					DX11Context::GetDevice()->CreateShaderResourceView(result->tex2D, &srvDesc, &result->resourceView);
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

				texDesc.Format = DXTypeMap(Desc.Format);
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.SampleDesc.Count = 1;
				texDesc.SampleDesc.Quality = 0;
				texDesc.CPUAccessFlags = 0;
				texDesc.ArraySize = 6;
				texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
				texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

				if (Desc.GenerateMipMaps == false)
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
		}
	}
}
#endif