// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "alimer_internal.h"
#include <stdio.h>

ALIMER_DISABLE_WARNINGS()
#define STBI_ASSERT(x) ALIMER_ASSERT(x)
#define STBI_MALLOC(sz) alimerMalloc(sz)
#define STBI_REALLOC(p,newsz) alimerRealloc(p, newsz)
#define STBI_FREE(p) alimerFree(p)
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_STDIO
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#define STBIW_ASSERT(x) ALIMER_ASSERT(x)
#define STBIW_MALLOC(sz) alimerMalloc(sz)
#define STBIW_REALLOC(p, newsz) alimerRealloc(p, newsz)
#define STBIW_FREE(p) alimerFree(p)
#define STBI_WRITE_NO_STDIO
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

#define QOI_NO_STDIO
#define QOI_IMPLEMENTATION
#define QOI_MALLOC(sz) STBI_MALLOC(sz)
#define QOI_FREE(p) STBI_FREE(p) 
#include "third_party/qoi.h"

#define TINYEXR_USE_MINIZ 0
#define TINYEXR_USE_STB_ZLIB 1
#define TINYEXR_IMPLEMENTATION
#include "third_party/tinyexr.h"
ALIMER_ENABLE_WARNINGS()

struct Image
{
    ImageDimension dimension;
    PixelFormat format;
    uint32_t width;
    uint32_t height;
    uint32_t depthOrArrayLayers;
    uint32_t mipLevelCount;
    size_t dataSize;
    void* pData;
};

// Format mapping table. The rows must be in the exactly same order as Format enum members are defined.
const PixelFormatInfo kFormatDesc[] = {
    //        format                   bytes blk         kind               
    { PixelFormat_Undefined,           0,   0, 0, PixelFormatKind_Uint },
    { PixelFormat_R8Unorm,             1,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_R8Snorm,             1,   1, 1, PixelFormatKind_Snorm },
    { PixelFormat_R8Uint,              1,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_R8Sint,              1,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_R16Unorm,            2,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_R16Snorm,            2,   1, 1, PixelFormatKind_Snorm },
    { PixelFormat_R16Uint,             2,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_R16Sint,             2,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_R16Float,            2,   1, 1, PixelFormatKind_Float },
    { PixelFormat_RG8Unorm,            2,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_RG8Snorm,            2,   1, 1, PixelFormatKind_Snorm },
    { PixelFormat_RG8Uint,             2,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_RG8Sint,             2,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_BGRA4Unorm,          2,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_B5G6R5Unorm,         2,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_BGR5A1Unorm,         2,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_R32Uint,             4,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_R32Sint,             4,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_R32Float,            4,   1, 1, PixelFormatKind_Float },
    { PixelFormat_RG16Unorm,           4,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_RG16Snorm,           4,   1, 1, PixelFormatKind_Snorm },
    { PixelFormat_RG16Uint,            4,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_RG16Sint,            4,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_RG16Float,           4,   1, 1, PixelFormatKind_Float },
    { PixelFormat_RGBA8Unorm,          4,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_RGBA8UnormSrgb,      4,   1, 1, PixelFormatKind_UnormSrgb },
    { PixelFormat_RGBA8Snorm,          4,   1, 1, PixelFormatKind_Snorm },
    { PixelFormat_RGBA8Uint,           4,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_RGBA8Sint,           4,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_BGRA8Unorm,          4,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_BGRA8UnormSrgb,      4,   1, 1, PixelFormatKind_UnormSrgb},
    { PixelFormat_RGB10A2Unorm,        4,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_RGB10A2Uint,         4,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_RG11B10UFloat,       4,   1, 1, PixelFormatKind_Float },
    { PixelFormat_RGB9E5UFloat,        4,   1, 1, PixelFormatKind_Float },
    { PixelFormat_RG32Uint,            8,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_RG32Sint,            8,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_RG32Float,           8,   1, 1, PixelFormatKind_Float },
    { PixelFormat_RGBA16Unorm,         8,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_RGBA16Snorm,         8,   1, 1, PixelFormatKind_Snorm },
    { PixelFormat_RGBA16Uint,          8,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_RGBA16Sint,          8,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_RGBA16Float,         8,   1, 1, PixelFormatKind_Float },
    { PixelFormat_RGBA32Uint,          16,   1, 1, PixelFormatKind_Uint },
    { PixelFormat_RGBA32Sint,          16,   1, 1, PixelFormatKind_Sint },
    { PixelFormat_RGBA32Float,         16,   1, 1, PixelFormatKind_Float },

    // Depth-stencil formats
    //{ PixelFormat_Stencil8,              4,   1, 1, PixelFormatKind::Uint },
    { PixelFormat_Depth16Unorm,            2,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_Depth24UnormStencil8,    4,   1, 1, PixelFormatKind_Unorm },
    { PixelFormat_Depth32Float,            4,   1, 1, PixelFormatKind_Float },
    { PixelFormat_Depth32FloatStencil8,    8,   1, 1, PixelFormatKind_Float },

    // BC compressed formats
    { PixelFormat_BC1RGBAUnorm,            8,   4, 4, PixelFormatKind_Unorm },
    { PixelFormat_BC1RGBAUnormSrgb,        8,   4, 4, PixelFormatKind_UnormSrgb },
    { PixelFormat_BC2RGBAUnorm,            16,  4, 4, PixelFormatKind_Unorm },
    { PixelFormat_BC2RGBAUnormSrgb,        16,  4, 4, PixelFormatKind_UnormSrgb },
    { PixelFormat_BC3RGBAUnorm,            16,  4, 4, PixelFormatKind_Unorm },
    { PixelFormat_BC3RGBAUnormSrgb,        16,  4, 4, PixelFormatKind_UnormSrgb  },
    { PixelFormat_BC4RUnorm,               8,   4, 4, PixelFormatKind_Unorm },
    { PixelFormat_BC4RSnorm,               8,   4, 4, PixelFormatKind_Snorm },
    { PixelFormat_BC5RGUnorm,              16,  4, 4, PixelFormatKind_Unorm },
    { PixelFormat_BC5RGSnorm,              16,  4, 4, PixelFormatKind_Snorm },
    { PixelFormat_BC6HRGBUfloat,           16,  4, 4, PixelFormatKind_Float },
    { PixelFormat_BC6HRGBFloat,            16,  4, 4, PixelFormatKind_Float },
    { PixelFormat_BC7RGBAUnorm,            16,  4, 4, PixelFormatKind_Unorm },
    { PixelFormat_BC7RGBAUnormSrgb,        16,  4, 4, PixelFormatKind_UnormSrgb },

    // ETC2/EAC compressed formats
    { PixelFormat_ETC2RGB8Unorm,           8,   4, 4, PixelFormatKind_Unorm },
    { PixelFormat_ETC2RGB8UnormSrgb,       8,   4, 4, PixelFormatKind_UnormSrgb },
    { PixelFormat_ETC2RGB8A1Unorm,         16,   4, 4, PixelFormatKind_Unorm },
    { PixelFormat_ETC2RGB8A1UnormSrgb,     16,   4, 4, PixelFormatKind_UnormSrgb },
    { PixelFormat_ETC2RGBA8Unorm,          16,   4, 4, PixelFormatKind_Unorm },
    { PixelFormat_ETC2RGBA8UnormSrgb,      16,   4, 4, PixelFormatKind_UnormSrgb },
    { PixelFormat_EACR11Unorm,             8,    4, 4, PixelFormatKind_Unorm },
    { PixelFormat_EACR11Snorm,             8,    4, 4, PixelFormatKind_Snorm },
    { PixelFormat_EACRG11Unorm,            16,   4, 4, PixelFormatKind_Unorm },
    { PixelFormat_EACRG11Snorm,            16,   4, 4, PixelFormatKind_Snorm },

    // ASTC compressed formats
    { PixelFormat_ASTC4x4Unorm,            16,   4, 4, PixelFormatKind_Unorm },
    { PixelFormat_ASTC4x4UnormSrgb,        16,   4, 4, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC5x4Unorm,            16,   5, 4, PixelFormatKind_Unorm },
    { PixelFormat_ASTC5x4UnormSrgb,        16,   5, 4, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC5x5Unorm,            16,   5, 5, PixelFormatKind_Unorm },
    { PixelFormat_ASTC5x5UnormSrgb,        16,   5, 5, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC6x5Unorm,            16,   6, 5, PixelFormatKind_Unorm },
    { PixelFormat_ASTC6x5UnormSrgb,        16,   6, 5, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC6x6Unorm,            16,   6, 6, PixelFormatKind_Unorm },
    { PixelFormat_ASTC6x6UnormSrgb,        16,   6, 6, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC8x5Unorm,            16,   8, 5, PixelFormatKind_Unorm },
    { PixelFormat_ASTC8x5UnormSrgb,        16,   8, 5, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC8x6Unorm,            16,   8, 6, PixelFormatKind_Unorm },
    { PixelFormat_ASTC8x6UnormSrgb,        16,   8, 6, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC8x8Unorm,            16,   8, 8, PixelFormatKind_Unorm },
    { PixelFormat_ASTC8x8UnormSrgb,        16,   8, 8, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC10x5Unorm,           16,   10, 5, PixelFormatKind_Unorm },
    { PixelFormat_ASTC10x5UnormSrgb,       16,   10, 5, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC10x6Unorm,           16,   10, 6, PixelFormatKind_Unorm },
    { PixelFormat_ASTC10x6UnormSrgb,       16,   10, 6, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC10x8Unorm,           16,   10, 8, PixelFormatKind_Unorm },
    { PixelFormat_ASTC10x8UnormSrgb,       16,   10, 8, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC10x10Unorm,          16,   10, 10, PixelFormatKind_Unorm  },
    { PixelFormat_ASTC10x10UnormSrgb,      16,   10, 10, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC12x10Unorm,          16,   12, 10, PixelFormatKind_Unorm  },
    { PixelFormat_ASTC12x10UnormSrgb,      16,   12, 10, PixelFormatKind_UnormSrgb },
    { PixelFormat_ASTC12x12Unorm,          16,   12, 12, PixelFormatKind_Unorm  },
    { PixelFormat_ASTC12x12UnormSrgb,      16,   12, 12, PixelFormatKind_UnormSrgb },
};

static_assert(
    sizeof(kFormatDesc) / sizeof(PixelFormatInfo) == size_t(_PixelFormat_Count),
    "The format info table doesn't have the right number of elements"
    );

bool GetPixelFormatInfo(PixelFormat format, PixelFormatInfo* info)
{
    if (format >= _PixelFormat_Count)
        return false;

    *info = kFormatDesc[(uint32_t)format];
    ALIMER_ASSERT(info->format == format);
    return true;
}


uint32_t GetFormatBytesPerBlock(PixelFormat format)
{
    if (format >= _PixelFormat_Count)
        return 0;

    const PixelFormatInfo info = kFormatDesc[(uint32_t)format];
    ALIMER_ASSERT(info.format == format);
    return info.bytesPerBlock;
}

bool IsDepthFormat(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat_Depth16Unorm:
        case PixelFormat_Depth24UnormStencil8:
        case PixelFormat_Depth32Float:
        case PixelFormat_Depth32FloatStencil8:
            return true;
        default:
            return false;
    }
}

bool IsStencilFormat(PixelFormat format)
{
    switch (format)
    {
        //case PixelFormat_Stencil8:
        case PixelFormat_Depth24UnormStencil8:
        case PixelFormat_Depth32FloatStencil8:
            return true;
        default:
            return false;
    }
}

bool IsDepthStencilFormat(PixelFormat format)
{
    switch (format)
    {
        //case PixelFormat_Stencil8:
        case PixelFormat_Depth16Unorm:
        case PixelFormat_Depth24UnormStencil8:
        case PixelFormat_Depth32Float:
        case PixelFormat_Depth32FloatStencil8:
            return true;
        default:
            return false;
    }
}

bool IsDepthOnlyFormat(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat_Depth16Unorm:
        case PixelFormat_Depth32Float:
            return true;
        default:
            return false;
    }
}

bool IsCompressedFormat(PixelFormat format)
{
    if (format >= _PixelFormat_Count)
        return 0;

    const PixelFormatInfo info = kFormatDesc[(uint32_t)format];
    ALIMER_ASSERT(info.format == format);
    return info.blockWidth > 1 || info.blockHeight > 1;
}

bool IsBCCompressedFormat(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat_BC1RGBAUnorm:
        case PixelFormat_BC1RGBAUnormSrgb:
        case PixelFormat_BC2RGBAUnorm:
        case PixelFormat_BC2RGBAUnormSrgb:
        case PixelFormat_BC3RGBAUnorm:
        case PixelFormat_BC3RGBAUnormSrgb:
        case PixelFormat_BC4RUnorm:
        case PixelFormat_BC4RSnorm:
        case PixelFormat_BC5RGUnorm:
        case PixelFormat_BC5RGSnorm:
        case PixelFormat_BC6HRGBUfloat:
        case PixelFormat_BC6HRGBFloat:
        case PixelFormat_BC7RGBAUnorm:
        case PixelFormat_BC7RGBAUnormSrgb:
            return true;
        default:
            return false;
    }
}

bool IsASTCCompressedFormat(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat_ASTC4x4Unorm:
        case PixelFormat_ASTC4x4UnormSrgb:
        case PixelFormat_ASTC5x4Unorm:
        case PixelFormat_ASTC5x4UnormSrgb:
        case PixelFormat_ASTC5x5Unorm:
        case PixelFormat_ASTC5x5UnormSrgb:
        case PixelFormat_ASTC6x5Unorm:
        case PixelFormat_ASTC6x5UnormSrgb:
        case PixelFormat_ASTC6x6Unorm:
        case PixelFormat_ASTC6x6UnormSrgb:
        case PixelFormat_ASTC8x5Unorm:
        case PixelFormat_ASTC8x5UnormSrgb:
        case PixelFormat_ASTC8x6Unorm:
        case PixelFormat_ASTC8x6UnormSrgb:
        case PixelFormat_ASTC8x8Unorm:
        case PixelFormat_ASTC8x8UnormSrgb:
        case PixelFormat_ASTC10x5Unorm:
        case PixelFormat_ASTC10x5UnormSrgb:
        case PixelFormat_ASTC10x6Unorm:
        case PixelFormat_ASTC10x6UnormSrgb:
        case PixelFormat_ASTC10x8Unorm:
        case PixelFormat_ASTC10x8UnormSrgb:
        case PixelFormat_ASTC10x10Unorm:
        case PixelFormat_ASTC10x10UnormSrgb:
        case PixelFormat_ASTC12x10Unorm:
        case PixelFormat_ASTC12x10UnormSrgb:
        case PixelFormat_ASTC12x12Unorm:
            return true;
        default:
            return false;
    }
}

PixelFormatKind GetPixelFormatKind(PixelFormat format)
{
    if (format >= _PixelFormat_Count)
        return _PixelFormatKind_Force32;

    const PixelFormatInfo info = kFormatDesc[(uint32_t)format];
    ALIMER_ASSERT(info.format == format);
    return info.kind;
}

bool IsIntegerFormat(PixelFormat format)
{
    if (format >= _PixelFormat_Count)
        return false;

    const PixelFormatInfo info = kFormatDesc[(uint32_t)format];
    ALIMER_ASSERT(info.format == format);
    return info.kind == PixelFormatKind_Uint || info.kind == PixelFormatKind_Sint;
}

bool IsSrgbFormat(PixelFormat format)
{
    if (format >= _PixelFormat_Count)
        return false;

    const PixelFormatInfo info = kFormatDesc[(uint32_t)format];
    ALIMER_ASSERT(info.format == format);
    return info.kind == PixelFormatKind_UnormSrgb;
}

PixelFormat SrgbToLinearFormat(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat_RGBA8UnormSrgb:
            return PixelFormat_RGBA8Unorm;
        case PixelFormat_BGRA8UnormSrgb:
            return PixelFormat_BGRA8Unorm;
            // Bc compressed formats
        case PixelFormat_BC1RGBAUnormSrgb:
            return PixelFormat_BC1RGBAUnorm;
        case PixelFormat_BC2RGBAUnormSrgb:
            return PixelFormat_BC2RGBAUnorm;
        case PixelFormat_BC3RGBAUnormSrgb:
            return PixelFormat_BC3RGBAUnorm;
        case PixelFormat_BC7RGBAUnormSrgb:
            return PixelFormat_BC7RGBAUnorm;

            // Etc2/Eac compressed formats
        case PixelFormat_ETC2RGB8UnormSrgb:
            return PixelFormat_ETC2RGB8Unorm;
        case PixelFormat_ETC2RGB8A1UnormSrgb:
            return PixelFormat_ETC2RGB8A1Unorm;
        case PixelFormat_ETC2RGBA8UnormSrgb:
            return PixelFormat_ETC2RGBA8Unorm;

            // Astc compressed formats
        case PixelFormat_ASTC4x4UnormSrgb:
            return PixelFormat_ASTC4x4Unorm;
        case PixelFormat_ASTC5x4UnormSrgb:
            return PixelFormat_ASTC5x4Unorm;
        case PixelFormat_ASTC5x5UnormSrgb:
            return PixelFormat_ASTC5x5Unorm;
        case PixelFormat_ASTC6x5UnormSrgb:
            return PixelFormat_ASTC6x5Unorm;
        case PixelFormat_ASTC6x6UnormSrgb:
            return PixelFormat_ASTC6x6Unorm;
        case PixelFormat_ASTC8x5UnormSrgb:
            return PixelFormat_ASTC8x5Unorm;
        case PixelFormat_ASTC8x6UnormSrgb:
            return PixelFormat_ASTC8x6Unorm;
        case PixelFormat_ASTC8x8UnormSrgb:
            return PixelFormat_ASTC8x8Unorm;
        case PixelFormat_ASTC10x5UnormSrgb:
            return PixelFormat_ASTC10x5Unorm;
        case PixelFormat_ASTC10x6UnormSrgb:
            return PixelFormat_ASTC10x6Unorm;
        case PixelFormat_ASTC10x8UnormSrgb:
            return PixelFormat_ASTC10x8Unorm;
        case PixelFormat_ASTC10x10UnormSrgb:
            return PixelFormat_ASTC10x10Unorm;
        case PixelFormat_ASTC12x10UnormSrgb:
            return PixelFormat_ASTC12x10Unorm;
        case PixelFormat_ASTC12x12UnormSrgb:
            return PixelFormat_ASTC12x12Unorm;

        default:
            ALIMER_ASSERT(IsSrgbFormat(format) == false);
            return format;
    }
}

PixelFormat LinearToSrgbFormat(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat_RGBA8Unorm:
            return PixelFormat_RGBA8UnormSrgb;
        case PixelFormat_BGRA8Unorm:
            return PixelFormat_BGRA8UnormSrgb;

            // Bc compressed formats
        case PixelFormat_BC1RGBAUnorm:
            return PixelFormat_BC1RGBAUnormSrgb;
        case PixelFormat_BC2RGBAUnorm:
            return PixelFormat_BC2RGBAUnormSrgb;
        case PixelFormat_BC3RGBAUnorm:
            return PixelFormat_BC3RGBAUnormSrgb;
        case PixelFormat_BC7RGBAUnorm:
            return PixelFormat_BC7RGBAUnormSrgb;

            // Etc2/Eac compressed formats
        case PixelFormat_ETC2RGB8Unorm:
            return PixelFormat_ETC2RGB8UnormSrgb;
        case PixelFormat_ETC2RGB8A1Unorm:
            return PixelFormat_ETC2RGB8A1UnormSrgb;
        case PixelFormat_ETC2RGBA8Unorm:
            return PixelFormat_ETC2RGBA8UnormSrgb;

            // Astc compressed formats
        case PixelFormat_ASTC4x4Unorm:
            return PixelFormat_ASTC4x4UnormSrgb;
        case PixelFormat_ASTC5x4Unorm:
            return PixelFormat_ASTC5x4UnormSrgb;
        case PixelFormat_ASTC5x5Unorm:
            return PixelFormat_ASTC5x5UnormSrgb;
        case PixelFormat_ASTC6x5Unorm:
            return PixelFormat_ASTC6x5UnormSrgb;
        case PixelFormat_ASTC6x6Unorm:
            return PixelFormat_ASTC6x6UnormSrgb;
        case PixelFormat_ASTC8x5Unorm:
            return PixelFormat_ASTC8x5UnormSrgb;
        case PixelFormat_ASTC8x6Unorm:
            return PixelFormat_ASTC8x6UnormSrgb;
        case PixelFormat_ASTC8x8Unorm:
            return PixelFormat_ASTC8x8UnormSrgb;
        case PixelFormat_ASTC10x5Unorm:
            return PixelFormat_ASTC10x5UnormSrgb;
        case PixelFormat_ASTC10x6Unorm:
            return PixelFormat_ASTC10x6UnormSrgb;
        case PixelFormat_ASTC10x8Unorm:
            return PixelFormat_ASTC10x8UnormSrgb;
        case PixelFormat_ASTC10x10Unorm:
            return PixelFormat_ASTC10x10UnormSrgb;
        case PixelFormat_ASTC12x10Unorm:
            return PixelFormat_ASTC12x10UnormSrgb;
        case PixelFormat_ASTC12x12Unorm:
            return PixelFormat_ASTC12x12UnormSrgb;

        default:
            return format;
    }
}

Image* alimerImageCreate2D(PixelFormat format, uint32_t width, uint32_t height, uint32_t arrayLayers, uint32_t mipLevelCount)
{
    if (!width || !height || !arrayLayers)
        return NULL;

    Image* image = ALIMER_ALLOC(Image);
    if (!image) {
        return nullptr;
    }

    image->dimension = ImageDimension_2D;
    image->format = format;
    image->width = width;
    image->height = height;
    image->depthOrArrayLayers = arrayLayers;
    image->mipLevelCount = mipLevelCount;

    return image;
}

Image* alimerImageCreateFromMemory(const void* pData, size_t dataSize)
{
    if (pData == nullptr || dataSize == 0)
        return nullptr;

    Image* image = ALIMER_ALLOC(Image);
    if (!image) {
        return nullptr;
    }

    return image;
}

void alimerImageDestroy(Image* image)
{
    if (!image)
        return;

    if (image->pData)
        alimerFree(image->pData);

    alimerFree(image);
}
