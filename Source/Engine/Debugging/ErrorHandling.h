//
// Created by joran on 05/08/2023.
//

#pragma once

#include <cmath>
#include <format>
#include <iostream>
#include <source_location>
#include <stdexcept>
#include <utility>
#include "Logging/Logger.h"
#include "Logging/EmptyLogger.h"

namespace FawnTrace
{
#ifdef _DEBUG
    using Log = FawnTrace::Logger;
#else
    using Log = FawnTrace::EmptyLogger;
#endif

    // Custom exception class
    class Exception : public std::exception
    {
    public:
        explicit Exception( std::string message, std::source_location loc = std::source_location::current() )
            : message( std::move( message ) )
            , loc( loc )
        {
        }

        [[nodiscard]] const char* what() const noexcept override
        {
            return std::format( R"({}\nFile: {}\nLine: {}\nFunction: {})", message, loc.file_name(), loc.line(), loc.function_name() ).c_str();
        }

    private:
        std::string message;
        std::source_location loc;
    };

    namespace Assert
    {
        // Assert function with optional message
        inline constexpr void Assert( bool condition, const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            if ( !condition )
            {
                throw Exception( message, loc );
            }
        }

        // Comparisons
        template<typename T>
        void AreApproximatelyEqual( const T& a, const T& b, const T& tolerance = std::numeric_limits<T>::epsilon(), const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( std::abs( a - b ) <= tolerance, message, loc );
        }

        template<typename T>
        void AreEqual( const T& a, const T& b, const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( a == b, message, loc );
        }

        template<typename T>
        void AreNotApproximatelyEqual( const T& a, const T& b, const T& tolerance = std::numeric_limits<T>::epsilon(), const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( std::abs( a - b ) > tolerance, message, loc );
        }

        template<typename T>
        void AreNotEqual( const T& a, const T& b, const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( a != b, message, loc );
        }

        // Boolean checks
        inline void IsTrue( bool condition, const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( condition, message, loc );
        }

        inline void IsFalse( bool condition, const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( !condition, message, loc );
        }

        // Pointers
        template<typename T>
        void IsNull( const T* ptr, const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( ptr == nullptr, message, loc );
        }

        template<typename T>
        void IsNotNull( const T* ptr, const std::string& message = "", std::source_location loc = std::source_location::current() )
        {
            Assert( ptr != nullptr, message, loc );
        }
    }// namespace Assert

}// namespace ErrorHandling
