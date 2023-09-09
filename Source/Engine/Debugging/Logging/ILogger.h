//
// Created by joran on 05/08/2023.
//

#pragma once

#include <cstdint>
#include <source_location>

namespace FawnTrace
{
    enum class LogLevel : uint8_t
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };
}

struct ILogger
{
    explicit ILogger()=default;
    virtual ~ILogger()=default;
    virtual void SetLogLevel( FawnTrace::LogLevel level ) = 0;
};