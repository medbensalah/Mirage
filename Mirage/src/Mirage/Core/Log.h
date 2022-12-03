#pragma once

#include "Mirage/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Mirage
{
    class Log
    {
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private :
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	return os << glm::to_string(quaternio);
}

/* Core Log */
#define MRG_CORE_TRACE(...)		::Mirage::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MRG_CORE_INFO(...)		::Mirage::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MRG_CORE_ERROR(...)		::Mirage::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MRG_CORE_WARN(...)		::Mirage::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MRG_CORE_CRITICAL(...)	::Mirage::Log::GetCoreLogger()->critical(__VA_ARGS__)

/* Client Log */
#define MRG_TRACE(...)			::Mirage::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MRG_INFO(...)			::Mirage::Log::GetClientLogger()->info(__VA_ARGS__)
#define MRG_ERROR(...)			::Mirage::Log::GetClientLogger()->error(__VA_ARGS__)
#define MRG_WARN(...)			::Mirage::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MRG_CRITICAL(...)	    ::Mirage::Log::GetClientLogger()->critical(__VA_ARGS__)
