// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "alimer_internal.h"

ALIMER_DISABLE_WARNINGS()
#define STBI_ASSERT(x) ALIMER_ASSERT(x)
//#define STBI_MALLOC(sz) alimer_alloc(sz)
//#define STBI_REALLOC(p,newsz) alimer_realloc(p, newsz)
//#define STBI_FREE(p) alimer_free(p)
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_STDIO
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#define STBIW_ASSERT(x) ALIMER_ASSERT(x)
//#define STBIW_MALLOC(sz) alimer_alloc(sz)
//#define STBIW_REALLOC(p, newsz) alimer_realloc(p, newsz)
//#define STBIW_FREE(p) alimer_free(p)
#define STBI_WRITE_NO_STDIO
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

#define QOI_NO_STDIO
#define QOI_IMPLEMENTATION
//#define QOI_MALLOC(sz) STBI_MALLOC(sz)
//#define QOI_FREE(p) STBI_FREE(p) 
#include "third_party/qoi.h"

#define TINYEXR_USE_MINIZ 0
#define TINYEXR_USE_STB_ZLIB 1
#define TINYEXR_IMPLEMENTATION
#include "third_party/tinyexr.h"
ALIMER_ENABLE_WARNINGS()
