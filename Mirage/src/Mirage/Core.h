#pragma once

#ifdef MRG_PLATFORM_WINDOWS
    #ifdef MRG_BUILD_DLL
        #define MIRAGE_CLASS class __declspec(dllexport)
    #else
        #define  MIRAGE_CLASS class __declspec(dllimport)
    #endif
#else
    #error MIRAGE SUPPORTS WINDOWS ONLY!!!
#endif

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