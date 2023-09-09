set(PROJECT_NAME AntlerSerialize)

add_library(${PROJECT_NAME} INTERFACE
        Serializer.h
)

set(ROOT_NAMESPACE ${PROJECT_NAME})
