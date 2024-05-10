// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "alimer_image.h"
#include <stdbool.h>
#include <stdlib.h> // malloc, free
#include <string.h> // memset
#include <assert.h>

struct alimerImage {
    PixelFormat format;
    size_t dataSize;
    void* pData;
};

alimerImage* alimer_CreateFromMemory(const void* pData, size_t dataSize)
{
    if (pData == NULL || dataSize == 0)
        return NULL;

    alimerImage* image = (alimerImage*)malloc(sizeof(alimerImage));
    assert(image);
    memset(image, 0, sizeof(alimerImage));

    return image;
}

alimerImage* AlimerImage_Create2D(PixelFormat format, uint32_t width, uint32_t height, uint32_t arrayLayers, uint32_t mipLevelCount)
{
    if (!width || !height || !arrayLayers)
        return NULL;

    alimerImage* image = (alimerImage*)malloc(sizeof(alimerImage));
    assert(image);
    memset(image, 0, sizeof(alimerImage));

    return image;
}

void alimerImage_Destroy(alimerImage* image)
{
    if (!image)
        return;

    if (image->pData)
        free(image->pData);

    free(image);
}
