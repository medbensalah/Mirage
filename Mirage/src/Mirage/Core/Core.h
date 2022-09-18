#pragma once

#include <memory>

#ifdef _WIN32
    /* Windows x64/x86 */
    #ifdef _WIN64
        /* Windows x64  */
        #define MRG_PLATFORM_WINDOWS
    #else
        /* Windows x86 */
        #error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all tMRG platforms
     * so we must cMRGck all of tMRGm (in this order)
     * to ensure that we're running on MAC
     * and not some otMRGr Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define MRG_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define MRG_PLATFORM_MACOS
        #error "MacOS is not supported!"
    #else
        #error "Unknown Apple platform!"
    #endif /* We also have to cMRGck __ANDROID__ before __linux__
            * since android is based on tMRG linux kernel
            * it has __linux__ defined */
#elif defined(__ANDROID__)
    #define MRG_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define MRG_PLATFORM_LINUX
    #error "Linux is not supported!"
#else
    /* Unknown compiler/platform */
    #error "Unknown platform!"
#endif // End of platform detection


// DLL support
#ifdef MRG_PLATFORM_WINDOWS
    #if MRG_DYNAMIC_LINK
        #ifdef MRG_BUILD_DLL
            #define MIRAGE_CLASS class __declspec(dllexport)
        #else
            #define MIRAGE_CLASS class __declspec(dllimport)
        #endif
    #else
        #define MIRAGE_CLASS class
    #endif
#else
    #error MIRAGE SUPPORTS WINDOWS ONLY!!!
#endif // End of DLL support

#ifdef MRG_DEBUG
    #define MRG_ENABLE_ASSERTS
#endif

#ifdef MRG_ENABLE_ASSERTS
    #define MRG_CORE_ASSERT(x, ...) { if(!(x)) { MRG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define MRG_ASSERT(x, ...) { if(!(x)) { MRG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define MRG_CORE_ASSERT(x, ...)
    #define MRG_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MRG_BIND_EVENT_FN(fn) std::bind(fn, this, std::placeholders::_1)

namespace Mirage
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
