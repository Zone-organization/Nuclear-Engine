#include <Assets/Image.h>
#include <Diligent\Graphics\GraphicsAccessories\interface\GraphicsAccessories.hpp>
#include <Diligent\Graphics\GraphicsTools\interface\GraphicsUtilities.h>
#include <Graphics\Context.h>
#include "..\ThirdParty\DiligentTextureLoader\src\TextureLoader.h"
#include <Assets/Importer.h>
#include <chrono>

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

		bool Image::Create()
		{			
			mTextureView = nullptr;

			mWidth = mDesc.mTexDesc.Width;
			mHeight = mDesc.mTexDesc.Height;

			//CREATE IMAGE
			Diligent::TextureData TexData;
			TexData.pSubResources = mDesc.mSubresources.data();
			TexData.NumSubresources = mDesc.mSubresources.size();
			RefCntAutoPtr<ITexture> mTexture;
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(mDesc.mTexDesc, &TexData, &mTexture);

			mDesc.mSubresources.clear();
			//mDesc.mSubresources.shrink_to_fit();
			std::vector<TextureSubResData>().swap(mDesc.mSubresources);

			mDesc.mMips.clear();
		//	mDesc.mMips.shrink_to_fit();
			std::vector<std::vector<Uint8>>().swap(mDesc.mMips);

			if (mTexture.RawPtr() != nullptr)
			{
				mTextureView = mTexture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
				SetState(IAsset::State::Created);
				return true;
			}

			return false;
		}
		ImageDesc& Image::GetDesc()
		{
			return mDesc;
		}
		void Image::ProcessImageData(const Assets::ImageData& data)
		{
			mDesc.mTexDesc.Type = data.mType;
			mDesc.mTexDesc.Usage = data.mUsage;
			mDesc.mTexDesc.BindFlags = data.mBindFlags;
			mDesc.mTexDesc.Format = data.mFormat;
			mDesc.mTexDesc.CPUAccessFlags = data.mCPUAccessFlags;
			mDesc.mTexDesc.Width = data.mWidth;
			mDesc.mTexDesc.Height = data.mHeight;

			if (mDesc.mSubresources.size() == 0)
			{
				mDesc.mTexDesc.MipLevels = ComputeMipLevelsCount(mDesc.mTexDesc.Width, mDesc.mTexDesc.Height);
				if (data.mMipLevels > 0)
					mDesc.mTexDesc.MipLevels = std::min(mDesc.mTexDesc.MipLevels, data.mMipLevels);

				Uint32 NumComponents = 0;
				const auto ChannelDepth = GetValueSize(data.mComponentType) * 8;

				bool IsSRGB = (data.mNumComponents >= 3 && ChannelDepth == 8) ? data.mIsSRGB : false;
				if (mDesc.mTexDesc.Format == TEX_FORMAT_UNKNOWN)
				{
					NumComponents = data.mNumComponents == 3 ? 4 : data.mNumComponents;

					if (ChannelDepth == 8)
					{
						switch (NumComponents)
						{
						case 1: mDesc.mTexDesc.Format = TEX_FORMAT_R8_UNORM; break;
						case 2: mDesc.mTexDesc.Format = TEX_FORMAT_RG8_UNORM; break;
						case 4: mDesc.mTexDesc.Format = IsSRGB ? TEX_FORMAT_RGBA8_UNORM_SRGB : TEX_FORMAT_RGBA8_UNORM; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", data.mNumComponents, ")");
						}
					}
					else if (ChannelDepth == 16)
					{
						switch (NumComponents)
						{
						case 1: mDesc.mTexDesc.Format = TEX_FORMAT_R16_UNORM; break;
						case 2: mDesc.mTexDesc.Format = TEX_FORMAT_RG16_UNORM; break;
						case 4: mDesc.mTexDesc.Format = TEX_FORMAT_RGBA16_UNORM; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", data.mNumComponents, ")");
						}
					}
					else if (ChannelDepth == 32)
					{
						switch (NumComponents)
						{
						case 1: mDesc.mTexDesc.Format = TEX_FORMAT_R16_FLOAT; break;
						case 2: mDesc.mTexDesc.Format = TEX_FORMAT_RG16_FLOAT; break;
						case 4: mDesc.mTexDesc.Format = TEX_FORMAT_RGBA32_FLOAT; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", data.mNumComponents, ")");
						}
					}
					else
						LOG_ERROR_AND_THROW("Unsupported color channel depth (", ChannelDepth, ")");
				}
				else
				{
					const auto& TexFmtDesc = GetTextureFormatAttribs(mDesc.mTexDesc.Format);

					NumComponents = TexFmtDesc.NumComponents;
					if (TexFmtDesc.ComponentSize != ChannelDepth / 8)
						LOG_ERROR_AND_THROW("Image channel size ", ChannelDepth, " is not compatible with texture format ", TexFmtDesc.Name);
				}
				mDesc.mSubresources = std::vector<TextureSubResData>(mDesc.mTexDesc.MipLevels);
				mDesc.mMips = std::vector< std::vector<Uint8> >(mDesc.mTexDesc.MipLevels);

				auto& pSubResources = mDesc.mSubresources;
				auto& Mips = mDesc.mMips;

				//std::vector< std::vector<Uint8> > Mips(mDesc.mTexDesc.MipLevels);

				if (data.mNumComponents != NumComponents)
				{
					auto DstStride = data.mWidth * NumComponents * ChannelDepth / 8;
					DstStride = AlignUp(DstStride, Uint32{ 4 });
					Mips[0].resize(size_t{ DstStride } *size_t{ data.mHeight });
					pSubResources[0].pData = Mips[0].data();
					pSubResources[0].Stride = DstStride;
					if (ChannelDepth == 8)
					{
						ModifyComponentCount<Uint8>(data.mData, data.mRowStride, data.mNumComponents,
							Mips[0].data(), DstStride,
							data.mWidth, data.mHeight, NumComponents);
					}
					else if (ChannelDepth == 16)
					{
						ModifyComponentCount<Uint16>(data.mData, data.mRowStride, data.mNumComponents,
							Mips[0].data(), DstStride,
							data.mWidth, data.mHeight, NumComponents);
					}
					else if (ChannelDepth == 32)
					{
						ModifyComponentCount<float>(data.mData, data.mRowStride, data.mNumComponents,
							Mips[0].data(), DstStride,
							data.mWidth, data.mHeight, NumComponents);
					}
				}
				else
				{
					auto MipLevelProps = GetMipLevelProperties(mDesc.mTexDesc, 0);

					pSubResources[0].pData = data.mData;
					pSubResources[0].Stride = MipLevelProps.RowSize;
				}

				for (Uint32 m = 1; m < mDesc.mTexDesc.MipLevels; ++m)
				{
					auto MipLevelProps = GetMipLevelProperties(mDesc.mTexDesc, m);
					Mips[m].resize(StaticCast<size_t>(MipLevelProps.MipSize));
					pSubResources[m].pData = Mips[m].data();
					pSubResources[m].Stride = MipLevelProps.RowSize;

					if (data.mGenerateMipMaps)
					{
						auto FinerMipProps = GetMipLevelProperties(mDesc.mTexDesc, m - 1);

						ComputeMipLevelAttribs Attribs;
						Attribs.Format = mDesc.mTexDesc.Format;
						Attribs.FineMipWidth = FinerMipProps.LogicalWidth;
						Attribs.FineMipHeight = FinerMipProps.LogicalHeight;
						Attribs.pFineMipData = pSubResources[m - 1].pData;
						Attribs.FineMipStride = StaticCast<size_t>(pSubResources[m - 1].Stride);
						Attribs.pCoarseMipData = Mips[m].data();
						Attribs.CoarseMipStride = StaticCast<size_t>(pSubResources[m].Stride);
						Attribs.AlphaCutoff = data.mMipMapsAlphaCutoff;
						Attribs.FilterType = data.mMipMapsFilter;
						ComputeMipLevel(Attribs);
					}
				}
			}
		}
		/*
		bool Image::Create(const Diligent::TextureDesc& TexDesc, const Diligent::TextureData& texdata)
		{
			RefCntAutoPtr<ITexture> texture;
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(TexDesc, &texdata, &texture);

			if (texture.RawPtr() != nullptr)
			{
				mTextureView = texture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
				SetState(IAsset::State::Created);
				return true;
			}
			return false;
		}*/
		void Image::SetTextureView(ITextureView* view)
		{
			mTextureView = view;
		}
		ITextureView* Image::GetTextureView()
		{
			return mTextureView.RawPtr();
		}
		const Uint32 Image::GetWidth() const
		{
			return mWidth;
		}
		const Uint32 Image::GetHeight() const
		{
			return mHeight;
		}
	}
}