#pragma once
#include <string>

class Logger
{
    enum class Level { Info, Warning, Error };

    static void Log(Level level, const std::string& msg) noexcept;

    Logger() = delete;

public:
    static inline void LogInfo(const std::string& info) noexcept { Log(Level::Info, info); }
    static inline void LogWarning(const std::string& warning) noexcept { Log(Level::Warning, warning); }
    static inline void LogError(const std::string& error) noexcept { Log(Level::Error, error); }
};