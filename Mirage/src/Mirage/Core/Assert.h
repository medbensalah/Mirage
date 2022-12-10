#pragma once

#include "Mirage/Core/Core.h"
#include "Mirage/Core/Log.h"
#include <filesystem>

#ifdef MRG_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define MRG_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { MRG##type##ERROR(msg, __VA_ARGS__); MRG_DEBUGBREAK(); } }
#define MRG_INTERNAL_ASSERT_WITH_MSG(type, check, ...) MRG_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define MRG_INTERNAL_ASSERT_NO_MSG(type, check) MRG_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}'", MRG_STRINGIFY_MACRO(check))

#define MRG_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define MRG_INTERNAL_ASSERT_GET_MACRO(...) MRG_EXPAND_MACRO( MRG_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, MRG_INTERNAL_ASSERT_WITH_MSG, MRG_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define MRG_ASSERT(...) MRG_EXPAND_MACRO( MRG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define MRG_CORE_ASSERT(...) MRG_EXPAND_MACRO( MRG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define MRG_ASSERT(...)
#define MRG_CORE_ASSERT(...)
#endif
