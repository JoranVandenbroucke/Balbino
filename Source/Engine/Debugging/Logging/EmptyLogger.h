//
// Created by joran on 05/08/2023.
//

#include "ILogger.h"

namespace FawnTrace
{
    class EmptyLogger : public ILogger
    {
    public:
        constexpr explicit EmptyLogger( LogLevel level = LogLevel::INFO )
        {
            (void)level;
        }

        ~EmptyLogger() override = default;

        void SetLogLevel( LogLevel level ) override
        {
            (void)level;
        }

        template<typename... Args>
        constexpr void Log( LogLevel level, const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            (void)level;
            (void)format;
            (void)loc;
        }

        template<typename... Args>
        constexpr void Debug( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            (void)format;
            (void)loc;
        }

        template<typename... Args>
        constexpr void Info( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            (void)format;
            (void)loc;
        }

        template<typename... Args>
        constexpr void Warning( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            (void)format;
            (void)loc;
        }

        template<typename... Args>
        constexpr void Error( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            (void)format;
            (void)loc;
        }

        template<typename... Args>
        constexpr void Fatal( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            (void)format;
            (void)loc;
        }
    };
}