set(PROJECT_NAME FawnTrace)

add_library(${PROJECT_NAME} INTERFACE
        ErrorHandling.h
        Logging/EmptyLogger.h
        Logging/ILogger.h
        Logging/Logger.h
)
set(ROOT_NAMESPACE ${PROJECT_NAME})
