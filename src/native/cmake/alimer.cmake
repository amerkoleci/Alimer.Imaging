# Determines target platform
if(EMSCRIPTEN)
    set(ALIMER_PLATFORM_NAME "Web")
elseif(ANDROID)
    set(ALIMER_PLATFORM_NAME "Android")
elseif(IOS)
    set(ALIMER_PLATFORM_NAME "iOS")
elseif(APPLE)
    set(ALIMER_PLATFORM_NAME "macOS")
elseif(WIN32)
    set(ALIMER_PLATFORM_NAME "Win32")
elseif(UNIX)
    set(ALIMER_PLATFORM_NAME "Unix")
else()
    message(FATAL_ERROR "Unrecognized platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Determines target architecture if not explicitly set
if(DEFINED VCPKG_TARGET_ARCHITECTURE)
    set(ALIMER_ARCH ${VCPKG_TARGET_ARCHITECTURE})
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Ww][Ii][Nn]32$")
    set(ALIMER_ARCH x86)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Xx]64$")
    set(ALIMER_ARCH x64)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]$")
    set(ALIMER_ARCH arm)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]64$")
    set(ALIMER_ARCH arm64)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]64EC$")
    set(ALIMER_ARCH arm64ec)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Ww][Ii][Nn]32$")
    set(ALIMER_ARCH x86)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Xx]64$")
    set(DIRECTX_ARCH x64)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]$")
    set(ALIMER_ARCH arm)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]64$")
    set(ALIMER_ARCH arm64)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]64EC$")
    set(ALIMER_ARCH arm64ec)
elseif(NOT (DEFINED ALIMER_ARCH))
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "[Aa][Rr][Mm]64|aarch64|arm64")
        set(ALIMER_ARCH arm64)
    else()
        set(ALIMER_ARCH x64)
    endif()
endif()

# Determines host architecture
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "[Aa][Rr][Mm]64|aarch64|arm64")
    set(ALIMER_HOST_ARCH arm64)
else()
    set(ALIMER_HOST_ARCH x64)
endif()

message(STATUS "Detected platform: ${ALIMER_PLATFORM_NAME}")
message(STATUS "Detected architecture: ${ALIMER_ARCH}")
message(STATUS "Detected host architecture: ${ALIMER_HOST_ARCH}")
