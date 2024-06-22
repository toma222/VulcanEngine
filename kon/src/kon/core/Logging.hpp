
#pragma once

#include "Object.hpp"
#include "kon/types/String.hpp"
#include "kon/core/Core.hpp"

#include <utility>
#include <cstdio>

namespace kon
{
    enum class LoggingLevel : int
    {
        trace,
        info,
        warn,
        error,
        fatal
    };

    // The logging subsystem
    class knAPI Logging : public Object
    {
    KN_OBJECT(Logging, Object)

    public:
        Logging();
        ~Logging();

        static void Write(int level, const char *s);

        template<typename ...Args>
        static constexpr void Write(const char *level, const char *file, int line, Args &&...args)
        {
            // File Time Urgency
            printf("[%s][%s:%i] ", level, file, line);
            printf(std::forward<Args>(args)...);
            printf("\n");
        }
    };
}

#define KN_TRACE(...) ::kon::Logging::Write("TRACE", __FILE__, __LINE__, __VA_ARGS__)
#define KN_INFO(...) ::kon::Logging::Write("INFO", __FILE__, __LINE__, __VA_ARGS__)
#define KN_WARN(...) ::kon::Logging::Write("WARN", __FILE__, __LINE__, __VA_ARGS__)
#define KN_ERROR(...) ::kon::Logging::Write("ERROR", __FILE__, __LINE__, __VA_ARGS__)

#define KN_LOG(...) printf("[TRACE][%i] ");
