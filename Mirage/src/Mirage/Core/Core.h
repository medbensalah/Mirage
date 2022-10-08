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
    #endif 
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

#ifdef MRG_DEBUG
    #if defined(MRG_PLATFORM_WINDOWS)
        #define MRG_DEBUGBREAK() __debugbreak()
    #elif defined(MRG_PLATFORM_LINUX)
        #include <signal.h>
        #define MRG_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define MRG_ENABLE_ASSERTS
    #else
        #define MRG_DEBUGBREAK()
#endif

#ifdef MRG_ENABLE_ASSERTS
    #define MRG_CORE_ASSERT(x, ...) { if(!(x)) { MRG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); MRG_DEBUGBREAK(); } }
    #define MRG_ASSERT(x, ...) { if(!(x)) { MRG_ERROR("Assertion Failed: {0}", __VA_ARGS__); MRG_DEBUGBREAK(); } }
#else
    #define MRG_CORE_ASSERT(x, ...)
    #define MRG_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MRG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define MRG_IMGUI_DRAW_LABEL_WIDGET(Label,offset,fn,...)    (ImGui::AlignTextToFramePadding(),ImGui::TextUnformatted(Label),ImGui::SameLine(ImMax(offset, 15 + ImGui::CalcTextSize(Label).x)),fn(__VA_ARGS__))

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

    template <typename T>
    using WeakRef = std::weak_ptr<T>;
}
