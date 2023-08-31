//
// Created by joran on 05/08/2023.
//

#include "ILogger.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
namespace FawnTrace
{
    class Logger : public ILogger
    {
    public:
        inline explicit Logger( LogLevel level = LogLevel::INFO )
            : logLevel { level }
            , logFile { "log.txt" }
        {
            fileStream.open( logFile.c_str(), std::ios_base::out | std::ios_base::app );
        }

        inline void SetLogLevel( LogLevel level ) override
        {
            logLevel = level;
        }

        template<typename... Args>
        inline void Log( LogLevel level, const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            if ( level >= logLevel )
            {
                std::ostringstream oss;
                LogMessage( oss, level );
                AppendTimestamp( oss );
                AppendLocation( oss, loc );
                if constexpr(sizeof...(Args) > 0)
                    AppendMessage( oss, format, std::forward<Args>( args )... );

                fileStream << oss.str() << std::endl;
                std::cout << oss.str() << std::endl;
            }

            if ( level == LogLevel::FATAL )
            {
                exit( 1 );
            }
        }

        template<typename... Args>
        constexpr void Debug( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            Log( LogLevel::DEBUG, format, loc, std::forward<Args>( args )... );
        }

        template<typename... Args>
        constexpr void Info( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            Log( LogLevel::INFO, format, loc, std::forward<Args>( args )... );
        }

        template<typename... Args>
        constexpr void Warning( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            Log( LogLevel::WARNING, format, loc, std::forward<Args>( args )... );
        }

        template<typename... Args>
        constexpr void Error( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            Log( LogLevel::ERROR, format, loc, std::forward<Args>( args )... );
        }

        template<typename... Args>
        constexpr void Fatal( const char* format, const std::source_location& loc = std::source_location::current(), Args&&... args )
        {
            Log( LogLevel::FATAL, format, loc, std::forward<Args>( args )... );
        }

    private:
        LogLevel logLevel;
        std::string logFile;
        std::ofstream fileStream;

        static inline void LogMessage( std::ostringstream& oss, LogLevel level )
        {
            auto now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
            std::tm timeInfo {};
            gmtime_s( &timeInfo, &now );
            char timeBuffer[ 80 ];
            std::strftime( timeBuffer, sizeof( timeBuffer ), "%Y-%m-%d %H:%M:%S", &timeInfo );
            oss << timeBuffer << " [" << LevelToString( level ) << "] ";
        }

        static inline void AppendTimestamp( std::ostringstream& oss )
        {
            auto now = std::chrono::system_clock::now();
            auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>( now.time_since_epoch() ) % 1000;
            oss << std::setw( 3 ) << std::setfill( '0' ) << ms.count() << " ";
        }

        static inline void AppendLocation( std::ostringstream& oss, const std::source_location& loc )
        {
            oss << loc.file_name() << ":" << loc.line() << " (" << loc.function_name() << ") ";
        }

        template<typename T>
        constexpr void AppendMessage( std::ostringstream& oss, const char* format, T&& arg )
        {
            while ( *format )
            {
                if ( *format == '%' && *( ++format ) != '%' )
                {
                    oss << arg;
                    ++format;
                }
                else
                {
                    oss << *format++;
                }
            }
        }

        template<typename T, typename... Args>
        constexpr void AppendMessage( std::ostringstream& oss, const char* format, T&& arg, Args&&... args )
        {
            while ( *format )
            {
                if ( *format == '%' && *( ++format ) != '%' )
                {
                    oss << arg;
                    AppendMessage( oss, format, std::forward<Args>( args )... );
                    return;
                }
                else
                {
                    oss << *format++;
                }
            }
        }

        static inline std::string LevelToString( LogLevel level )
        {
            switch ( level )
            {
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO: return "INFO";
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                case LogLevel::FATAL: return "FATAL";
                default: return "";
            }
        }
    };
}