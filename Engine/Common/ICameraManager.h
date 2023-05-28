//
// Created by joran on 22/10/2022.
//

#pragma once
class CCameraComponent;

class ICameraManager
{
public:
    ICameraManager() = default;
    virtual ~ICameraManager() = default;
    ICameraManager( const ICameraManager& ) = delete;
    ICameraManager( ICameraManager&& ) = delete;
    virtual ICameraManager& operator=( const ICameraManager& ) = delete;
    virtual ICameraManager& operator=( ICameraManager&& ) = delete;
    
    virtual void Initialize() = 0;
    virtual void AddCamera( CCameraComponent* pComponent ) = 0;
    virtual void SwitchCameraToActive( CCameraComponent* pComponent ) = 0;
    virtual void RemoveCamera( CCameraComponent* pComponent ) = 0;
    virtual bool IsThisCameraActive( const CCameraComponent* pComponent ) = 0;
    [[nodiscard]] virtual const CCameraComponent* GetActiveCamera() const = 0;
    virtual CCameraComponent* GetActiveCamera() = 0;
    virtual void Cleanup() = 0;
};


