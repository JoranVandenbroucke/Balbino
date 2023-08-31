set(PROJECT_NAME BalbinoComponent)

add_library(${PROJECT_NAME} INTERFACE
        CameraComponent.h
        IDComponent.h
        LightComponent.h
        MeshRendererComponent.h
        TransformComponent.h
)

add_dependencies(${PROJECT_NAME}
        BambiMath
)

set(ROOT_NAMESPACE ${PROJECT_NAME})
