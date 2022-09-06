#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Mirage
{
    MIRAGE_CLASS Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private :
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

/* Core Log */
#define MRG_CORE_TRACE(...)		::Mirage::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MRG_CORE_INFO(...)		::Mirage::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MRG_CORE_ERROR(...)		::Mirage::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MRG_CORE_WARN(...)		::Mirage::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MRG_CORE_FATAL(...)		::Mirage::Log::GetCoreLogger()->fatal(__VA_ARGS__)

/* Client Log */
#define MRG_TRACE(...)			::Mirage::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MRG_INFO(...)			::Mirage::Log::GetClientLogger()->info(__VA_ARGS__)
#define MRG_ERROR(...)			::Mirage::Log::GetClientLogger()->error(__VA_ARGS__)
#define MRG_WARN(...)			::Mirage::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MRG_FATAL(...)			::Mirage::Log::GetClientLogger()->fatal(__VA_ARGS__)
