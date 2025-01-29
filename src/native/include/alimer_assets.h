// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#ifndef _ALIMER_ASSETS_H
#define _ALIMER_ASSETS_H

#if defined(ALIMER_SHARED_LIBRARY)
#    if defined(_WIN32)
#        if defined(ALIMER_IMPLEMENTATION)
#            define _ALIMER_EXPORT __declspec(dllexport)
#        else
#            define _ALIMER_EXPORT __declspec(dllimport)
#        endif
#    else
#        if defined(ALIMER_IMPLEMENTATION)
#            define _ALIMER_EXPORT __attribute__((visibility("default")))
#        else
#            define _ALIMER_EXPORT
#        endif
#    endif
#else
#    define _ALIMER_EXPORT
#endif

#ifdef __cplusplus
#    define _ALIMER_EXTERN extern "C"
#else
#    define _ALIMER_EXTERN extern
#endif

#define ALIMER_API _ALIMER_EXTERN _ALIMER_EXPORT 

#ifdef _WIN32
#   define ALIMER_CALL __cdecl
#else
#   define ALIMER_CALL
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Image Image;
typedef struct Font Font;

typedef enum PixelFormat {
	PixelFormat_Undefined = 0,
	// 8-bit formats
	PixelFormat_R8Unorm,
	PixelFormat_R8Snorm,
	PixelFormat_R8Uint,
	PixelFormat_R8Sint,
	// 16-bit formats
	PixelFormat_R16Unorm,
	PixelFormat_R16Snorm,
	PixelFormat_R16Uint,
	PixelFormat_R16Sint,
	PixelFormat_R16Float,
	PixelFormat_RG8Unorm,
	PixelFormat_RG8Snorm,
	PixelFormat_RG8Uint,
	PixelFormat_RG8Sint,
	// Packed 16-Bit formats
	PixelFormat_BGRA4Unorm,
	PixelFormat_B5G6R5Unorm,
	PixelFormat_BGR5A1Unorm,
	// 32-bit formats
	PixelFormat_R32Uint,
	PixelFormat_R32Sint,
	PixelFormat_R32Float,
	PixelFormat_RG16Unorm,
	PixelFormat_RG16Snorm,
	PixelFormat_RG16Uint,
	PixelFormat_RG16Sint,
	PixelFormat_RG16Float,
	PixelFormat_RGBA8Unorm,
	PixelFormat_RGBA8UnormSrgb,
	PixelFormat_RGBA8Snorm,
	PixelFormat_RGBA8Uint,
	PixelFormat_RGBA8Sint,
	PixelFormat_BGRA8Unorm,
	PixelFormat_BGRA8UnormSrgb,
	// Packed 32-Bit Pixel Formats
	PixelFormat_RGB10A2Unorm,
	PixelFormat_RGB10A2Uint,
	PixelFormat_RG11B10UFloat,
	PixelFormat_RGB9E5UFloat,
	// 64-bit formats
	PixelFormat_RG32Uint,
	PixelFormat_RG32Sint,
	PixelFormat_RG32Float,
	PixelFormat_RGBA16Unorm,
	PixelFormat_RGBA16Snorm,
	PixelFormat_RGBA16Uint,
	PixelFormat_RGBA16Sint,
	PixelFormat_RGBA16Float,
	// 128-bit formats
	PixelFormat_RGBA32Uint,
	PixelFormat_RGBA32Sint,
	PixelFormat_RGBA32Float,
	// Depth-stencil formats
	PixelFormat_Depth16Unorm,
	PixelFormat_Depth24UnormStencil8,
	PixelFormat_Depth32Float,
	PixelFormat_Depth32FloatStencil8,
	// BC compressed formats
	PixelFormat_BC1RGBAUnorm,
	PixelFormat_BC1RGBAUnormSrgb,
	PixelFormat_BC2RGBAUnorm,
	PixelFormat_BC2RGBAUnormSrgb,
	PixelFormat_BC3RGBAUnorm,
	PixelFormat_BC3RGBAUnormSrgb,
	PixelFormat_BC4RUnorm,
	PixelFormat_BC4RSnorm,
	PixelFormat_BC5RGUnorm,
	PixelFormat_BC5RGSnorm,
	PixelFormat_BC6HRGBUfloat,
	PixelFormat_BC6HRGBFloat,
	PixelFormat_BC7RGBAUnorm,
	PixelFormat_BC7RGBAUnormSrgb,
	// ETC2/EAC compressed formats
	PixelFormat_ETC2RGB8Unorm,
	PixelFormat_ETC2RGB8UnormSrgb,
	PixelFormat_ETC2RGB8A1Unorm,
	PixelFormat_ETC2RGB8A1UnormSrgb,
	PixelFormat_ETC2RGBA8Unorm,
	PixelFormat_ETC2RGBA8UnormSrgb,
	PixelFormat_EACR11Unorm,
	PixelFormat_EACR11Snorm,
	PixelFormat_EACRG11Unorm,
	PixelFormat_EACRG11Snorm,
	// ASTC compressed formats
	PixelFormat_ASTC4x4Unorm,
	PixelFormat_ASTC4x4UnormSrgb,
	PixelFormat_ASTC5x4Unorm,
	PixelFormat_ASTC5x4UnormSrgb,
	PixelFormat_ASTC5x5Unorm,
	PixelFormat_ASTC5x5UnormSrgb,
	PixelFormat_ASTC6x5Unorm,
	PixelFormat_ASTC6x5UnormSrgb,
	PixelFormat_ASTC6x6Unorm,
	PixelFormat_ASTC6x6UnormSrgb,
	PixelFormat_ASTC8x5Unorm,
	PixelFormat_ASTC8x5UnormSrgb,
	PixelFormat_ASTC8x6Unorm,
	PixelFormat_ASTC8x6UnormSrgb,
	PixelFormat_ASTC8x8Unorm,
	PixelFormat_ASTC8x8UnormSrgb,
	PixelFormat_ASTC10x5Unorm,
	PixelFormat_ASTC10x5UnormSrgb,
	PixelFormat_ASTC10x6Unorm,
	PixelFormat_ASTC10x6UnormSrgb,
	PixelFormat_ASTC10x8Unorm,
	PixelFormat_ASTC10x8UnormSrgb,
	PixelFormat_ASTC10x10Unorm,
	PixelFormat_ASTC10x10UnormSrgb,
	PixelFormat_ASTC12x10Unorm,
	PixelFormat_ASTC12x10UnormSrgb,
	PixelFormat_ASTC12x12Unorm,
	PixelFormat_ASTC12x12UnormSrgb,

	_PixelFormat_Count,
	_PixelFormat_Force32 = 0x7FFFFFFF
} PixelFormat;

typedef enum PixelFormatKind {
	/// Unsigned normalized formats
	PixelFormatKind_Unorm,
	/// Unsigned normalized sRGB formats
	PixelFormatKind_UnormSrgb,
	/// Signed normalized formats
	PixelFormatKind_Snorm,
	/// Unsigned integer formats
	PixelFormatKind_Uint,
	/// Unsigned integer formats
	PixelFormatKind_Sint,
	/// Floating-point formats
	PixelFormatKind_Float,

	_PixelFormatKind_Count,
	_PixelFormatKind_Force32 = 0x7FFFFFFF
} PixelFormatKind;

typedef enum ImageDimension {
	/// One-dimensional Texture.
	ImageDimension_1D = 0,
	/// Two-dimensional Texture.
	ImageDimension_2D = 1,
	/// Three-dimensional Texture.
	ImageDimension_3D = 2,
	/// Cubemap Texture.
	ImageDimension_Cube = 3,

	_ImageDimension_Count,
	_ImageDimensiont_Force32 = 0x7FFFFFFF
} ImageDimension;

typedef struct PixelFormatInfo {
	PixelFormat format;
	uint8_t bytesPerBlock;
	uint8_t blockWidth;
	uint8_t blockHeight;
	PixelFormatKind kind;
} PixelFormatInfo;

ALIMER_API bool GetPixelFormatInfo(PixelFormat format, PixelFormatInfo* info);

/// Get the number of bytes per format.
ALIMER_API uint32_t GetFormatBytesPerBlock(PixelFormat format);

/// Check if the format has a depth component
ALIMER_API bool IsDepthFormat(PixelFormat format);

/// Check if the format has a stencil component
ALIMER_API bool IsStencilFormat(PixelFormat format);

/// Check if the format has depth or stencil components
ALIMER_API bool IsDepthStencilFormat(PixelFormat format);

/// Check if the format has a depth only component.
ALIMER_API bool IsDepthOnlyFormat(PixelFormat format);

/// Check if the format is a compressed format.
ALIMER_API bool IsCompressedFormat(PixelFormat format);

/// Check if the format is a BC-compressed format.
ALIMER_API bool IsBCCompressedFormat(PixelFormat format);

/// Check if the format is a ASTC-compressed format.
ALIMER_API bool IsASTCCompressedFormat(PixelFormat format);

/// Get the pixel format kind
ALIMER_API PixelFormatKind GetPixelFormatKind(PixelFormat format);

/// Check if a format is an integer type.
ALIMER_API bool IsIntegerFormat(PixelFormat format);

/// Check if a format represents sRGB color space
ALIMER_API bool IsSrgbFormat(PixelFormat format);

/// Convert an SRGB format to linear. If the format is already linear, will return it
ALIMER_API PixelFormat SrgbToLinearFormat(PixelFormat format);

/// Convert an linear format to sRGB. If the format doesn't have a matching sRGB format, will return the original
ALIMER_API PixelFormat LinearToSrgbFormat(PixelFormat format);

/* Image */
ALIMER_API Image* alimerImageCreate2D(PixelFormat format, uint32_t width, uint32_t height, uint32_t arrayLayers, uint32_t mipLevelCount);
ALIMER_API Image* alimerImageCreateFromMemory(const void* pData, size_t dataSize);
ALIMER_API void alimerImageDestroy(Image* image);

/* Font */
ALIMER_API Font* alimerFontCreateFromMemory(const uint8_t* data, size_t size);
ALIMER_API void alimerFontDestroy(Font* font);
ALIMER_API void alimerFontGetMetrics(Font* font, int* ascent, int* descent, int* linegap);
ALIMER_API int alimerFontGetGlyphIndex(Font* font, int codepoint);
ALIMER_API float alimerFontGetScale(Font* font, float size);
ALIMER_API float alimerFontGetKerning(Font* font, int glyph1, int glyph2, float scale);
ALIMER_API void alimerFontGetCharacter(Font* font, int glyph, float scale, int* width, int* height, float* advance, float* offsetX, float* offsetY, int* visible);
ALIMER_API void alimerFontGetPixels(Font* font, uint8_t* dest, int glyph, int width, int height, float scale);

#endif /* _ALIMER_ASSETS_H */
