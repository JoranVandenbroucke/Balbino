//
// Created by joran on 03/08/2023.
//

#pragma once
#include <functional>
#include <vector>

#include <catch2/catch_all.hpp>

class StartupFunctionManager
{
public:
    // Function type for startup functions
    typedef std::function<void()> Function;

    static StartupFunctionManager& GetInstance()
    {
        static StartupFunctionManager instance;
        return instance;
    }

    // Add a new startup function to the manager
    void AddStartupFunction( const Function& function )
    {
        m_startupFunctions.push_back( function );
    }

    // Add a new startup function to the manager
    void AddShutdownFunction( const Function& function )
    {
        m_shutdownFunctions.push_back( function );
    }

    // Call all registered startup functions
    void CallAllStartupFunctions()
    {
        for ( const auto& function : m_startupFunctions )
        {
            function();
        }
    }

    // Call all registered shutdown functions
    void CallAllShutdownFunctions()
    {
        for ( const auto& function : m_shutdownFunctions )
        {
            function();
        }
    }

private:
    std::vector<Function> m_startupFunctions;
    std::vector<Function> m_shutdownFunctions;
};

#define START( function )                                                                                                                                                                                                                                      \
    static void START_##function();                                                                                                                                                                                                                            \
    namespace                                                                                                                                                                                                                                                  \
    {                                                                                                                                                                                                                                                          \
        struct START_##function##_Initializer {                                                                                                                                                                                                                \
            START_##function##_Initializer()                                                                                                                                                                                                                   \
            {                                                                                                                                                                                                                                                  \
                StartupFunctionManager::GetInstance().AddStartupFunction( START_##function );                                                                                                                                                                  \
            }                                                                                                                                                                                                                                                  \
        };                                                                                                                                                                                                                                                     \
        START_##function##_Initializer start_##function##_initializer;                                                                                                                                                                                         \
    }                                                                                                                                                                                                                                                          \
    static void START_##function()

#define STOP( function )                                                                                                                                                                                                                                       \
    static void STOP_##function();                                                                                                                                                                                                                             \
    namespace                                                                                                                                                                                                                                                  \
    {                                                                                                                                                                                                                                                          \
        struct STOP_##function##_Initializer {                                                                                                                                                                                                                 \
            STOP_##function##_Initializer()                                                                                                                                                                                                                    \
            {                                                                                                                                                                                                                                                  \
                StartupFunctionManager::GetInstance().AddShutdownFunction( STOP_##function );                                                                                                                                                                  \
            }                                                                                                                                                                                                                                                  \
        };                                                                                                                                                                                                                                                     \
        STOP_##function##_Initializer stop_##function##_initializer;                                                                                                                                                                                           \
    }                                                                                                                                                                                                                                                          \
    static void STOP_##function()
