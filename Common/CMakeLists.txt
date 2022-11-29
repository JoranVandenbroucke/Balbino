set(PROJECT_NAME Common)

file(GLOB headers
        *.h
        Components/*.h
        )

add_library(${PROJECT_NAME} INTERFACE ${headers})

