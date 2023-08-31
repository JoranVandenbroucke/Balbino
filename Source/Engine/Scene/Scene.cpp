#include "Scene.h"

#include <CameraComponent.h>

void BalbinoScene::CScene::Initialize()
{
#pragma todo("Load Scene From File, only create new camera when no file is found")  // todo: Load Scene From File, only create new camera when no file is found
    CreateEntity().AddComponent<BalbinoComponent::CCameraComponent>();
}
void BalbinoScene::CScene::Cleanup()
{
    m_registry.clear();
}
void BalbinoScene::CScene::Update( const float deltaTime )
{
    (void)deltaTime;
}
