#pragma once
#include "Core.h"
#include "ICameraManager.h"

struct IScene;
struct IResourceManager;

class ICameraManager;

struct ISystem
{
    virtual ~ISystem() = default;
    
    virtual bool Update( bool isPause = false ) = 0;
    
    [[nodiscard]] virtual IResourceManager* GetResourceManager() const = 0;
    [[nodiscard]] virtual ICameraManager* GetCameraManager() const = 0;
    [[nodiscard]] virtual IScene* GetCurrentActiveScene() const = 0;
    [[nodiscard]] virtual float GetWindowWidth() = 0;
    [[nodiscard]] virtual float GetWindowHeight() = 0;
};
