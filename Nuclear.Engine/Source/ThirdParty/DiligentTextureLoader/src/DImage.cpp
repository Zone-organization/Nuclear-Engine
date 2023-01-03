/*
 *  Copyright 2019-2022 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */


#include <algorithm>
#include <array>

#include "Image.h"
#include <Diligent/Primitives/interface/Errors.hpp>

#include <Diligent/Platforms/Basic/interface/DebugUtilities.hpp>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Common/interface/DataBlobImpl.hpp>
#include <Diligent/Common/interface/BasicFileStream.hpp>
#include <Diligent/Common/interface/StringTools.hpp>

#include <Diligent/Graphics/GraphicsAccessories/interface/GraphicsAccessories.hpp>
#include <Diligent/Common/interface/Align.hpp>

namespace Diligent
{

//void Image::CreateFromDataBlob(IDataBlob*           pFileData,
//                               const ImageLoadInfo& LoadInfo,
//                               Image**              ppImage)
//{
//    *ppImage = MakeNewRCObj<Image>()(pFileData, LoadInfo);
//    (*ppImage)->AddRef();
//}

Image::Image(IReferenceCounters* pRefCounters,
             const ImageData&    Desc,
             IDataBlob*          pPixels) :
    TBase{pRefCounters},
    m_Desc{Desc},
    m_pData{pPixels}
{
}




static const std::array<Uint8, 4> GetRGBAOffsets(TEXTURE_FORMAT Format)
{
    switch (Format)
    {
        case TEX_FORMAT_BGRA8_TYPELESS:
        case TEX_FORMAT_BGRA8_UNORM:
        case TEX_FORMAT_BGRA8_UNORM_SRGB:
            return {{2, 1, 0, 3}};
        default:
            return {{0, 1, 2, 3}};
    }
}

std::vector<Uint8> Image::ConvertImageData(Uint32         Width,
                                           Uint32         Height,
                                           const Uint8*   pData,
                                           Uint32         Stride,
                                           TEXTURE_FORMAT SrcFormat,
                                           TEXTURE_FORMAT DstFormat,
                                           bool           KeepAlpha)
{
    const auto& SrcFmtAttribs = GetTextureFormatAttribs(SrcFormat);
    const auto& DstFmtAttribs = GetTextureFormatAttribs(DstFormat);
    VERIFY(SrcFmtAttribs.ComponentSize == 1, "Only 8-bit formats are currently supported");
    VERIFY(DstFmtAttribs.ComponentSize == 1, "Only 8-bit formats are currently supported");

    auto NumDstComponents = SrcFmtAttribs.NumComponents;
    if (!KeepAlpha)
        NumDstComponents = std::min(NumDstComponents, Uint8{3});

    auto SrcOffsets = GetRGBAOffsets(SrcFormat);
    auto DstOffsets = GetRGBAOffsets(DstFormat);

    std::vector<Uint8> ConvertedData(size_t{DstFmtAttribs.ComponentSize} * size_t{NumDstComponents} * Width * Height);

    for (size_t j = 0; j < Height; ++j)
    {
        for (size_t i = 0; i < Width; ++i)
        {
            for (Uint32 c = 0; c < NumDstComponents; ++c)
            {
                ConvertedData[j * Width * NumDstComponents + i * NumDstComponents + DstOffsets[c]] =
                    pData[j * Stride + i * SrcFmtAttribs.NumComponents + SrcOffsets[c]];
            }
        }
    }

    return ConvertedData;
}

IMAGE_FILE_FORMAT Image::GetFileFormat(const Uint8* pData, size_t Size, const char* FilePath)
{
    if (pData != nullptr)
    {
        if (Size >= 3 && pData[0] == 0xFF && pData[1] == 0xD8 && pData[2] == 0xFF)
            return IMAGE_FILE_FORMAT_JPEG;

        if (Size >= 8 &&
            pData[0] == 0x89 && pData[1] == 0x50 && pData[2] == 0x4E && pData[3] == 0x47 &&
            pData[4] == 0x0D && pData[5] == 0x0A && pData[6] == 0x1A && pData[7] == 0x0A)
            return IMAGE_FILE_FORMAT_PNG;

        if (Size >= 4 &&
            ((pData[0] == 0x49 && pData[1] == 0x20 && pData[2] == 0x49) ||
                (pData[0] == 0x49 && pData[1] == 0x49 && pData[2] == 0x2A && pData[3] == 0x00) ||
                (pData[0] == 0x4D && pData[1] == 0x4D && pData[2] == 0x00 && pData[3] == 0x2A) ||
                (pData[0] == 0x4D && pData[1] == 0x4D && pData[2] == 0x00 && pData[3] == 0x2B)))
            return IMAGE_FILE_FORMAT_TIFF;

        if (Size >= 4 && pData[0] == 0x44 && pData[1] == 0x44 && pData[2] == 0x53 && pData[3] == 0x20)
            return IMAGE_FILE_FORMAT_DDS;

        static constexpr Uint8 KTX10FileIdentifier[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };
        static constexpr Uint8 KTX20FileIdentifier[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x32, 0x30, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };
        if (Size >= 12 &&
            (memcmp(pData, KTX10FileIdentifier, sizeof(KTX10FileIdentifier)) == 0 ||
                memcmp(pData, KTX20FileIdentifier, sizeof(KTX20FileIdentifier)) == 0))
            return IMAGE_FILE_FORMAT_KTX;

        if (Size >= 2 && pData[0] == 0x01 && pData[1] == 0xDA)
            return IMAGE_FILE_FORMAT_SGI;
    }

    if (FilePath != nullptr)
    {
        // Try to use extension to derive format
        auto* pDotPos = strrchr(FilePath, '.');
        if (pDotPos == nullptr)
        {
            LOG_ERROR_MESSAGE("Unable to recognize file format: file name '", FilePath, "' does not contain extension");
            return IMAGE_FILE_FORMAT_UNKNOWN;
        }

        auto* pExtension = pDotPos + 1;
        if (*pExtension == 0)
        {
            LOG_ERROR_MESSAGE("Unable to recognize file format: file name '", FilePath, "' contain empty extension");
            return IMAGE_FILE_FORMAT_UNKNOWN;
        }

        String Extension = StrToLower(pExtension);
        if (Extension == "png")
            return IMAGE_FILE_FORMAT_PNG;
        else if (Extension == "jpeg" || Extension == "jpg")
            return IMAGE_FILE_FORMAT_JPEG;
        else if (Extension == "tiff" || Extension == "tif")
            return IMAGE_FILE_FORMAT_TIFF;
        else if (Extension == "dds")
            return IMAGE_FILE_FORMAT_DDS;
        else if (Extension == "ktx")
            return IMAGE_FILE_FORMAT_KTX;
        else if (Extension == "sgi" || Extension == "rgb" || Extension == "rgba" || Extension == "bw" || Extension == "int" || Extension == "inta")
            return IMAGE_FILE_FORMAT_SGI;
        else
            LOG_ERROR_MESSAGE("Unrecognized image file extension", Extension);
    }

    return IMAGE_FILE_FORMAT_UNKNOWN;
}

} // namespace Diligent
