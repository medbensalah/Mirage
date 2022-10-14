#pragma once

#include <memory>
#include "Mirage/Core/PlatformDetection.h"

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

#define MRG_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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
