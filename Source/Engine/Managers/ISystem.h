#pragma once
#include "Core.h"
#include "ICameraManager.h"
#include <ErrorHandling.h>

struct IResourceManager;
struct ICameraManager;

namespace BalbinoScene
{
    class CScene;
}
namespace FawnVision
{
    class CRenderer;
}

struct ISystem {
    virtual ~ISystem() = default;

    virtual bool Update( bool isPause ) = 0;

    [[nodiscard]] virtual BalbinoScene::CScene* GetCurrentActiveScene() const noexcept = 0;
    [[nodiscard]] virtual IResourceManager* GetResourceManager() const noexcept        = 0;
    [[nodiscard]] virtual FawnVision::CRenderer* GetRenderer() const noexcept          = 0;
    [[nodiscard]] virtual ICameraManager* GetCameraManager() const noexcept            = 0;
    [[nodiscard]] virtual int GetWindowHeight() noexcept                               = 0;
    [[nodiscard]] virtual int GetWindowWidth() noexcept                                = 0;
    FawnTrace::Log Log {};
};
