// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#ifndef _ALIMER_IMAGING_H
#define _ALIMER_IMAGING_H

#if defined(ALIMER_IMAGE_SHARED_LIBRARY)
#    if defined(_WIN32)
#        if defined(ALIMER_IMAGE_IMPLEMENTATION)
#            define _ALIMER_IMAGE_EXPORT __declspec(dllexport)
#        else
#            define _ALIMER_IMAGE_EXPORT __declspec(dllimport)
#        endif
#    else
#        if defined(ALIMER_IMAGE_IMPLEMENTATION)
#            define _ALIMER_IMAGE_EXPORT __attribute__((visibility("default")))
#        else
#            define _ALIMER_IMAGE_EXPORT
#        endif
#    endif
#else
#    define _ALIMER_IMAGE_EXPORT
#endif

#ifdef __cplusplus
#    define _ALIMER_IMAGE_EXTERN extern "C"
#else
#    define _ALIMER_IMAGE_EXTERN extern
#endif

#define ALIMER_IMAGE_API _ALIMER_IMAGE_EXTERN _ALIMER_IMAGE_EXPORT 

#ifdef _WIN32
#   define ALIMER_IMAGE_CALL __cdecl
#else
#   define ALIMER_IMAGE_CALL
#endif

#include <stddef.h>
#include <stdint.h>

typedef uint32_t Bool32;
typedef struct alimerImage alimerImage;

typedef enum PixelFormat {
	_PixelFormat_Count,
	_PixelFormat_Force32 = 0x7FFFFFFF
} PixelFormat;

ALIMER_IMAGE_API alimerImage* alimer_CreateFromMemory(const void* pData, size_t dataSize);
ALIMER_IMAGE_API alimerImage* AlimerImage_Create2D(PixelFormat format, uint32_t width, uint32_t height, uint32_t arrayLayers, uint32_t mipLevelCount);
ALIMER_IMAGE_API void alimerImage_Destroy(alimerImage* image);

#endif /* _ALIMER_IMAGING_H */
