#pragma once
#include <Diligent\Graphics\GraphicsAccessories\interface\GraphicsAccessories.hpp>
#include <Engine\Graphics\Context.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine/Importers/Importers.h>

namespace NuclearEngine
{
	namespace Managers
	{
		namespace Internal
		{
			static const float a = 0.055f;

			// https://en.wikipedia.org/wiki/SRGB
			float SRGBToLinear(float SRGB)
			{
				if (SRGB < 0.04045f)
					return SRGB / 12.92f;
				else
					return pow((SRGB + a) / (1 + a), 2.4f);
			}

			float LinearToSRGB(float c)
			{
				if (c < 0.0031308f)
					return 12.92f * c;
				else
					return (1 + a) * pow(c, 1.f / 2.4f) - a;
			}

			template<typename ChannelType>
			ChannelType SRGBAverage(ChannelType c0, ChannelType c1, ChannelType c2, ChannelType c3)
			{
				constexpr float NormVal = static_cast<float>(std::numeric_limits<ChannelType>::max());
				float fc0 = static_cast<float>(c0) / NormVal;
				float fc1 = static_cast<float>(c1) / NormVal;
				float fc2 = static_cast<float>(c2) / NormVal;
				float fc3 = static_cast<float>(c3) / NormVal;

				float fLinearAverage = (SRGBToLinear(fc0) + SRGBToLinear(fc1) + SRGBToLinear(fc2) + SRGBToLinear(fc3)) / 4.f;
				float fSRGBAverage = LinearToSRGB(fLinearAverage);
				Int32 SRGBAverage = static_cast<Int32>(fSRGBAverage * NormVal);
				SRGBAverage = std::min(SRGBAverage, static_cast<Int32>(std::numeric_limits<ChannelType>::max()));
				SRGBAverage = std::max(SRGBAverage, static_cast<Int32>(std::numeric_limits<ChannelType>::min()));
				return static_cast<ChannelType>(SRGBAverage);
			}

			template < typename ChannelType >
			void ComputeCoarseMip(Uint32 NumChannels, bool IsSRGB,
				const void* pFineMip,
				Uint32      FineMipStride,
				void*       pCoarseMip,
				Uint32      CoarseMipStride,
				Uint32      CoarseMipWidth,
				Uint32      CoarseMipHeight)
			{
				for (size_t row = 0; row < size_t{ CoarseMipHeight }; ++row)
					for (size_t col = 0; col < size_t{ CoarseMipWidth }; ++col)
					{
						auto FineRow0 = reinterpret_cast<const ChannelType*>(reinterpret_cast<const Uint8*>(pFineMip) + row * 2 * size_t{ FineMipStride });
						auto FineRow1 = reinterpret_cast<const ChannelType*>(reinterpret_cast<const Uint8*>(pFineMip) + (row * 2 + 1) * size_t { FineMipStride });

						for (Uint32 c = 0; c < NumChannels; ++c)
						{
							auto Col00 = FineRow0[col * 2 * NumChannels + c];
							auto Col01 = FineRow0[(col * 2 + 1) * NumChannels + c];
							auto Col10 = FineRow1[col * 2 * NumChannels + c];
							auto Col11 = FineRow1[(col * 2 + 1) * NumChannels + c];
							auto &DstCol = reinterpret_cast<ChannelType*>(reinterpret_cast<Uint8*>(pCoarseMip) + row * size_t{ CoarseMipStride })[col * NumChannels + c];
							if (IsSRGB)
								DstCol = SRGBAverage(Col00, Col01, Col10, Col11);
							else
								DstCol = (Col00 + Col01 + Col10 + Col11) / 4;
						}
					}
			}

			template < typename ChannelType >
			void RGBToRGBA(const void*  pRGBData,
				Uint32       RGBStride,
				void*        pRGBAData,
				Uint32       RGBAStride,
				Uint32       Width,
				Uint32       Height)
			{
				for (size_t row = 0; row < size_t{ Height }; ++row)
					for (size_t col = 0; col < size_t{ Width }; ++col)
					{
						for (int c = 0; c < 3; ++c)
						{
							reinterpret_cast<ChannelType*>((reinterpret_cast<Uint8*>(pRGBAData) + size_t{ RGBAStride } *row))[col * 4 + c] =
								reinterpret_cast<const ChannelType*>((reinterpret_cast<const Uint8*>(pRGBData) + size_t{ RGBStride } *row))[col * 3 + c];
						}
						reinterpret_cast<ChannelType*>((reinterpret_cast<Uint8*>(pRGBAData) + size_t{ RGBAStride } *row))[col * 4 + 3] = std::numeric_limits<ChannelType>::max();
					}
			}

			bool CreateTextureFromRawImage(const Assets::Image& Image, const Importers::TextureLoadingDesc& Desc, Assets::Texture& result)
			{	
				TextureDesc TexDesc;
				TexDesc.Type = RESOURCE_DIM_TEX_2D;
				TexDesc.Width = Image.mWidth;
				TexDesc.Height = Image.mHeight;
				TexDesc.MipLevels = ComputeMipLevelsCount(TexDesc.Width, TexDesc.Height);
				if (Desc.mMipLevels > 0)
					TexDesc.MipLevels = std::min(TexDesc.MipLevels, Desc.mMipLevels);
				TexDesc.Usage = Desc.mUsage;
				TexDesc.BindFlags = Desc.mBindFlags;
				TexDesc.Format = Desc.mFormat;
				TexDesc.CPUAccessFlags = Desc.mCPUAccessFlags;

				auto ChannelDepth = Image.mBitsPerPixel / Image.mNumComponents;
				Uint32 NumComponents = Image.mNumComponents == 3 ? 4 : Image.mNumComponents;

				bool IsSRGB = (Image.mNumComponents >= 3 && ChannelDepth == 8) ? Desc.mIsSRGB : false;
				if (TexDesc.Format == TEX_FORMAT_UNKNOWN)
				{
					if (ChannelDepth == 8)
					{
						switch (NumComponents)
						{
						case 1: TexDesc.Format = TEX_FORMAT_R8_UNORM; break;
						case 2: TexDesc.Format = TEX_FORMAT_RG8_UNORM; break;
						case 4: TexDesc.Format = IsSRGB ? TEX_FORMAT_RGBA8_UNORM_SRGB : TEX_FORMAT_RGBA8_UNORM; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", Image.mNumComponents, ")");
						}
					}
					else if (ChannelDepth == 16)
					{
						switch (NumComponents)
						{
						case 1: TexDesc.Format = TEX_FORMAT_R16_UNORM; break;
						case 2: TexDesc.Format = TEX_FORMAT_RG16_UNORM; break;
						case 4: TexDesc.Format = TEX_FORMAT_RGBA16_UNORM; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", Image.mNumComponents, ")");
						}
					}
					else
						LOG_ERROR_AND_THROW("Unsupported color channel depth (", ChannelDepth, ")");
				}
				else
				{
					const auto& TexFmtDesc = GetTextureFormatAttribs(TexDesc.Format);
					if (TexFmtDesc.NumComponents != NumComponents)
						LOG_ERROR_AND_THROW("Incorrect number of components ", Image.mNumComponents, ") for texture format ", TexFmtDesc.Name);
					if (TexFmtDesc.ComponentSize != ChannelDepth / 8)
						LOG_ERROR_AND_THROW("Incorrect channel size ", ChannelDepth, ") for texture format ", TexFmtDesc.Name);
				}


				std::vector<TextureSubResData> pSubResources(TexDesc.MipLevels);
				std::vector< std::vector<Uint8> > Mips(TexDesc.MipLevels);

				if (Image.mNumComponents == 3)
				{
					VERIFY_EXPR(NumComponents == 4);
					auto RGBAStride = Image.mWidth * NumComponents * ChannelDepth / 8;
					RGBAStride = (RGBAStride + 3) & (-4);
					Mips[0].resize(size_t{ RGBAStride } *size_t{ Image.mHeight });
					pSubResources[0].pData = Mips[0].data();
					pSubResources[0].Stride = RGBAStride;
					if (ChannelDepth == 8)
						RGBToRGBA<Uint8>(Image.mData, Image.mRowStride,
							Mips[0].data(), RGBAStride,
							Image.mWidth, Image.mHeight);
					else if (ChannelDepth == 16)
						RGBToRGBA<Uint16>(Image.mData, Image.mRowStride,
							Mips[0].data(), RGBAStride,
							Image.mWidth , Image.mHeight);
				}
				else
				{
					pSubResources[0].pData = Image.mData;
					pSubResources[0].Stride = Image.mRowStride;
				}

				auto MipWidth = TexDesc.Width;
				auto MipHeight = TexDesc.Height;
				for (Uint32 m = 1; m < TexDesc.MipLevels; ++m)
				{
					auto CoarseMipWidth = std::max(MipWidth / 2u, 1u);
					auto CoarseMipHeight = std::max(MipHeight / 2u, 1u);
					auto CoarseMipStride = CoarseMipWidth * NumComponents * ChannelDepth / 8;
					CoarseMipStride = (CoarseMipStride + 3) & (-4);
					Mips[m].resize(size_t{ CoarseMipStride } *size_t{ CoarseMipHeight });

					if (Desc.mGenerateMips)
					{
						if (ChannelDepth == 8)
							ComputeCoarseMip<Uint8>(NumComponents, IsSRGB,
								pSubResources[m - 1].pData, pSubResources[m - 1].Stride,
								Mips[m].data(), CoarseMipStride,
								CoarseMipWidth, CoarseMipHeight);
						else if (ChannelDepth == 16)
							ComputeCoarseMip<Uint16>(NumComponents, IsSRGB,
								pSubResources[m - 1].pData, pSubResources[m - 1].Stride,
								Mips[m].data(), CoarseMipStride,
								CoarseMipWidth, CoarseMipHeight);
					}

					pSubResources[m].pData = Mips[m].data();
					pSubResources[m].Stride = CoarseMipStride;

					MipWidth = CoarseMipWidth;
					MipHeight = CoarseMipHeight;
				}

				TextureData TexData;
				TexData.pSubResources = pSubResources.data();
				TexData.NumSubresources = TexDesc.MipLevels;
				RefCntAutoPtr<ITexture> mTexture;
				Graphics::Context::GetDevice()->CreateTexture(TexDesc, &TexData, &mTexture);
				if (mTexture.RawPtr() != nullptr)
				{
					result.mTextureView = mTexture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

					if (result.mTextureView.RawPtr() != nullptr)
						return true;
				}
				return false;
			}
		}
	}
}