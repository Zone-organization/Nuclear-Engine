#include <Assets/Image.h>
#include <Diligent\Graphics\GraphicsAccessories\interface\GraphicsAccessories.hpp>
#include <Diligent\Graphics\GraphicsTools\interface\GraphicsUtilities.h>
#include <Graphics\Context.h>

namespace Nuclear
{
	namespace Assets 
	{
		Image::Image()
			: IAsset(AssetType::Image)
		{
		}

		Image::~Image()
		{
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

		bool Image::Create(const ImageCreationDesc& Desc)
		{
			TextureDesc TexDesc;
			TexDesc.Type = Desc.mImportingDesc.mType;
			TexDesc.Width = Desc.mData.mWidth;
			TexDesc.Height = Desc.mData.mHeight;

			const auto  ChannelDepth = GetValueSize(Desc.mData.mComponentType) * 8;

			TexDesc.MipLevels = ComputeMipLevelsCount(TexDesc.Width, TexDesc.Height);
			if (Desc.mImportingDesc.mMipLevels > 0)
				TexDesc.MipLevels = std::min(TexDesc.MipLevels, Desc.mImportingDesc.mMipLevels);

			TexDesc.Usage = Desc.mImportingDesc.mUsage;
			TexDesc.BindFlags = Desc.mImportingDesc.mBindFlags;
			TexDesc.Format = TEX_FORMAT_UNKNOWN;
			TexDesc.CPUAccessFlags = Desc.mImportingDesc.mCPUAccessFlags;

			Uint32 NumComponents = 0;

			bool IsSRGB = (Desc.mData.mNumComponents >= 3 && ChannelDepth == 8) ? Desc.mImportingDesc.mIsSRGB : false;
			if (TexDesc.Format == TEX_FORMAT_UNKNOWN)
			{
				NumComponents = Desc.mData.mNumComponents == 3 ? 4 : Desc.mData.mNumComponents;

				if (ChannelDepth == 8)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R8_UNORM; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG8_UNORM; break;
					case 4: TexDesc.Format = IsSRGB ? TEX_FORMAT_RGBA8_UNORM_SRGB : TEX_FORMAT_RGBA8_UNORM; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", Desc.mData.mNumComponents, ")");
					}
				}
				else if (ChannelDepth == 16)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R16_UNORM; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG16_UNORM; break;
					case 4: TexDesc.Format = TEX_FORMAT_RGBA16_UNORM; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", Desc.mData.mNumComponents, ")");
					}
				}
				else if (ChannelDepth == 32)
				{
					switch (NumComponents)
					{
					case 1: TexDesc.Format = TEX_FORMAT_R16_FLOAT; break;
					case 2: TexDesc.Format = TEX_FORMAT_RG16_FLOAT; break;
					case 4: TexDesc.Format = TEX_FORMAT_RGBA32_FLOAT; break;
					default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", Desc.mData.mNumComponents, ")");
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

			if (Desc.mData.mNumComponents != NumComponents)
			{
				auto DstStride = Desc.mData.mWidth * NumComponents * ChannelDepth / 8;
				DstStride = AlignUp(DstStride, Uint32{ 4 });
				Mips[0].resize(size_t{ DstStride } *size_t{ Desc.mData.mHeight });
				pSubResources[0].pData = Mips[0].data();
				pSubResources[0].Stride = DstStride;
				if (ChannelDepth == 8)
				{
					ModifyComponentCount<Uint8>(Desc.mData.mData, Desc.mData.mRowStride, Desc.mData.mNumComponents,
						Mips[0].data(), DstStride,
						Desc.mData.mWidth, Desc.mData.mHeight, NumComponents);
				}
				else if (ChannelDepth == 16)
				{
					ModifyComponentCount<Uint16>(Desc.mData.mData, Desc.mData.mRowStride, Desc.mData.mNumComponents,
						Mips[0].data(), DstStride,
						Desc.mData.mWidth, Desc.mData.mHeight, NumComponents);
				}
				else if (ChannelDepth == 32)
				{
					ModifyComponentCount<float>(Desc.mData.mData, Desc.mData.mRowStride, Desc.mData.mNumComponents,
						Mips[0].data(), DstStride,
						Desc.mData.mWidth, Desc.mData.mHeight, NumComponents);
				}
			}
			else
			{
				auto MipLevelProps = GetMipLevelProperties(TexDesc, 0);

				pSubResources[0].pData = Desc.mData.mData;
				pSubResources[0].Stride = MipLevelProps.RowSize;
			}

			for (Uint32 m = 1; m < TexDesc.MipLevels; ++m)
			{
				auto MipLevelProps = GetMipLevelProperties(TexDesc, m);
				Mips[m].resize(StaticCast<size_t>(MipLevelProps.MipSize));
				pSubResources[m].pData = Mips[m].data();
				pSubResources[m].Stride = MipLevelProps.RowSize;

				if (Desc.mImportingDesc.mGenerateMips)
				{
					auto FinerMipProps = GetMipLevelProperties(TexDesc, m - 1);
					if (Desc.mImportingDesc.mGenerateMips)
					{
						ComputeMipLevelAttribs Attribs;
						Attribs.Format = TexDesc.Format;
						Attribs.FineMipWidth = FinerMipProps.LogicalWidth;
						Attribs.FineMipHeight = FinerMipProps.LogicalHeight;
						Attribs.pFineMipData = pSubResources[m - 1].pData;
						Attribs.FineMipStride = StaticCast<size_t>(pSubResources[m - 1].Stride);
						Attribs.pCoarseMipData = Mips[m].data();
						Attribs.CoarseMipStride = StaticCast<size_t>(pSubResources[m].Stride);
						Attribs.AlphaCutoff = Desc.mImportingDesc.AlphaCutoff;
						static_assert(MIP_FILTER_TYPE_DEFAULT == static_cast<MIP_FILTER_TYPE>(TEXTURE_LOAD_MIP_FILTER_DEFAULT), "Inconsistent enum values");
						static_assert(MIP_FILTER_TYPE_BOX_AVERAGE == static_cast<MIP_FILTER_TYPE>(TEXTURE_LOAD_MIP_FILTER_BOX_AVERAGE), "Inconsistent enum values");
						static_assert(MIP_FILTER_TYPE_MOST_FREQUENT == static_cast<MIP_FILTER_TYPE>(TEXTURE_LOAD_MIP_FILTER_MOST_FREQUENT), "Inconsistent enum values");
						Attribs.FilterType = static_cast<MIP_FILTER_TYPE>(Desc.mImportingDesc.MipFilter);
						ComputeMipLevel(Attribs);
					}
				}
			}

			TextureData TexData;
			TexData.pSubResources = pSubResources.data();
			TexData.NumSubresources = TexDesc.MipLevels;
			RefCntAutoPtr<ITexture> mTexture;
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(TexDesc, &TexData, &mTexture);

			if (mTexture.RawPtr() != nullptr)
			{
				mTextureView = mTexture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
				return true;
			}

			return false;
		}
		void Image::SetTextureView(ITextureView* view)
		{
			mTextureView = view;
		}
		ITextureView* Image::GetTextureView()
		{
			return mTextureView.RawPtr();
		}
	}
}