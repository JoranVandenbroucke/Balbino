//
// Created by joran on 22/10/2022.
//

#pragma once
namespace BalbinoComponent
{
    class CCameraComponent;
}
struct ICameraManager
{
public:
    ICameraManager()                                           = default;
    virtual ~ICameraManager()                                  = default;
    ICameraManager( const ICameraManager& )                    = delete;
    ICameraManager( ICameraManager&& )                         = delete;
    virtual ICameraManager& operator=( const ICameraManager& ) = delete;
    virtual ICameraManager& operator=( ICameraManager&& )      = delete;

    virtual void Initialize() = 0;
    virtual void Cleanup()    = 0;

    [[nodiscard]] virtual bool IsThisCameraActive( const BalbinoComponent::CCameraComponent* pComponent ) noexcept = 0;
    [[nodiscard]] virtual const BalbinoComponent::CCameraComponent* GetActiveCamera() const noexcept               = 0;
    [[nodiscard]] virtual BalbinoComponent::CCameraComponent* GetActiveCamera() noexcept                           = 0;

    virtual void AddCamera( BalbinoComponent::CCameraComponent* pComponent ) noexcept            = 0;
    virtual void SwitchCameraToActive( BalbinoComponent::CCameraComponent* pComponent ) noexcept = 0;
    virtual void RemoveCamera( BalbinoComponent::CCameraComponent* pComponent ) noexcept         = 0;
};
