#include <Engine/Assets/Image.h>
#include <Diligent\Graphics\GraphicsAccessories\interface\GraphicsAccessories.hpp>
#include <Engine\Graphics\Context.h>

namespace Nuclear
{
	namespace Assets 
	{
		Image::Image()
		{
		}

		Image::Image(const Assets::ImageData& src_data, const Importers::ImageLoadingDesc& Desc)
		{
			mData = src_data;
			CreateTextureFromRawImage(src_data, Desc);
		}

		Image::~Image()
		{
		}

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
			void* pCoarseMip,
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
						auto& DstCol = reinterpret_cast<ChannelType*>(reinterpret_cast<Uint8*>(pCoarseMip) + row * size_t{ CoarseMipStride })[col * NumChannels + c];
						if (IsSRGB)
							DstCol = SRGBAverage(Col00, Col01, Col10, Col11);
						else
							DstCol = (Col00 + Col01 + Col10 + Col11) / 4;
					}
				}
		}

		template <typename ChannelType>
		void ModifyComponentCount(const void* pSrcData,
			Uint32      SrcStride,
			Uint32      SrcCompCount,
			void* pDstData,
			Uint32      DstStride,
			Uint32      Width,
			Uint32      Height,
			Uint32      DstCompCount)
		{
			auto CompToCopy = std::min(SrcCompCount, DstCompCount);
			for (size_t row = 0; row < size_t{ Height }; ++row)
			{
				for (size_t col = 0; col < size_t{ Width }; ++col)
				{
					// clang-format off
					auto* pDst = reinterpret_cast<ChannelType*>((reinterpret_cast<Uint8*>(pDstData) + size_t{ DstStride } *row)) + col * DstCompCount;
					const auto* pSrc = reinterpret_cast<const ChannelType*>((reinterpret_cast<const Uint8*>(pSrcData) + size_t{ SrcStride } *row)) + col * SrcCompCount;
					// clang-format on

					for (size_t c = 0; c < CompToCopy; ++c)
						pDst[c] = pSrc[c];

					for (size_t c = CompToCopy; c < DstCompCount; ++c)
					{
						pDst[c] = c < 3 ?
							(SrcCompCount == 1 ? pSrc[0] : 0) :      // For single-channel source textures, propagate r to other channels
							std::numeric_limits<ChannelType>::max(); // Use 1.0 as default value for alpha
					}
				}
			}
		}

		bool Image::CreateTextureFromRawImage(const Assets::ImageData& src_data, const Importers::ImageLoadingDesc& Desc)
		{
			TextureDesc TexDesc;
			TexDesc.Type = Desc.mType;
			TexDesc.Width = src_data.mWidth;
			TexDesc.Height = src_data.mHeight;

			TexDesc.MipLevels = ComputeMipLevelsCount(TexDesc.Width, TexDesc.Height);
			if (Desc.mMipLevels > 0)
				TexDesc.MipLevels = std::min(TexDesc.MipLevels, Desc.mMipLevels);

			TexDesc.Usage = Desc.mUsage;
			TexDesc.BindFlags = Desc.mBindFlags;
			TexDesc.Format = TEX_FORMAT_UNKNOWN;
			TexDesc.CPUAccessFlags = Desc.mCPUAccessFlags;

			auto ChannelDepth = src_data.mBitsPerPixel / src_data.mNumComponents;
			Uint32 NumComponents = src_data.mNumComponents == 3 ? 4 : src_data.mNumComponents;

			bool IsSRGB = (src_data.mNumComponents >= 3 && ChannelDepth == 8) ? Desc.mIsSRGB : false;
			if (TexDesc.Format == TEX_FORMAT_UNKNOWN)
			{
				if (ChannelDepth == 8)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R8_UNORM; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG8_UNORM; break;
					case 4: TexDesc.Format = IsSRGB ? TEX_FORMAT_RGBA8_UNORM_SRGB : TEX_FORMAT_RGBA8_UNORM; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", src_data.mNumComponents, ")");
					}
				}
				else if (ChannelDepth == 16)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R16_UNORM; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG16_UNORM; break;
					case 4: TexDesc.Format = TEX_FORMAT_RGBA16_UNORM; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", src_data.mNumComponents, ")");
					}
				}
				else if (ChannelDepth == 32)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R16_FLOAT; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG16_FLOAT; break;
					case 4: TexDesc.Format = TEX_FORMAT_RGBA32_FLOAT; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", src_data.mNumComponents, ")");
					}
				}
				else
					LOG_ERROR_AND_THROW("Unsupported color channel depth (", ChannelDepth, ")");
			}
			else
			{
				const auto& TexFmtDesc = GetTextureFormatAttribs(TexDesc.Format);

				NumComponents = TexFmtDesc.NumComponents;
				if (TexFmtDesc.ComponentSize != ChannelDepth / 8)
					LOG_ERROR_AND_THROW("Image channel size ", ChannelDepth, " is not compatible with texture format ", TexFmtDesc.Name);
			}


			std::vector<TextureSubResData> pSubResources(TexDesc.MipLevels);
			std::vector< std::vector<Uint8> > Mips(TexDesc.MipLevels);

			if (src_data.mNumComponents != NumComponents)
			{
				auto DstStride = src_data.mWidth * NumComponents * ChannelDepth / 8;
				DstStride = AlignUp(DstStride, Uint32{ 4 });
				Mips[0].resize(size_t{ DstStride } *size_t{ src_data.mHeight });
				pSubResources[0].pData = Mips[0].data();
				pSubResources[0].Stride = DstStride;
				if (ChannelDepth == 8)
				{
					ModifyComponentCount<Uint8>(src_data.mData, src_data.mRowStride, src_data.mNumComponents,
						Mips[0].data(), DstStride,
						src_data.mWidth, src_data.mHeight, NumComponents);
				}
				else if (ChannelDepth == 16)
				{
					ModifyComponentCount<Uint16>(src_data.mData, src_data.mRowStride, src_data.mNumComponents,
						Mips[0].data(), DstStride,
						src_data.mWidth, src_data.mHeight, NumComponents);
				}
				else if (ChannelDepth == 32)
				{
					ModifyComponentCount<float>(src_data.mData, src_data.mRowStride, src_data.mNumComponents,
						Mips[0].data(), DstStride,
						src_data.mWidth, src_data.mHeight, NumComponents);
				}
			}
			else
			{
				pSubResources[0].pData = src_data.mData;
				pSubResources[0].Stride = src_data.mRowStride;
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
							pSubResources[m - 1].pData, static_cast<Uint32>(pSubResources[m - 1].Stride),
							Mips[m].data(), CoarseMipStride,
							CoarseMipWidth, CoarseMipHeight);
					else if (ChannelDepth == 16)
						ComputeCoarseMip<Uint16>(NumComponents, IsSRGB,
							pSubResources[m - 1].pData, static_cast<Uint32>(pSubResources[m - 1].Stride),
							Mips[m].data(), CoarseMipStride,
							CoarseMipWidth, CoarseMipHeight);
					else if (ChannelDepth == 32)
						ComputeCoarseMip<float>(NumComponents, IsSRGB,
							pSubResources[m - 1].pData, static_cast<Uint32>(pSubResources[m - 1].Stride),
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

			mData.mData = NULL;
			if (mTexture.RawPtr() != nullptr)
			{
				mTextureView = mTexture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
				return true;
			}

			return false;
		}
	}
}