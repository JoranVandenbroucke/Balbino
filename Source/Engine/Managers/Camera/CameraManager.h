//
// Created by joran on 22/10/2022.
//

#pragma once
#include "../ICameraManager.h"
#include <vector>

namespace BalbinoControl
{
    class CCameraManager final : public ICameraManager
    {
    public:
        CCameraManager()                                                    = default;
        ~CCameraManager() override                                          = default;
        CCameraManager( const CCameraManager& )                             = delete;
        CCameraManager( CCameraManager&& )                                  = delete;
        ICameraManager& operator=( const ICameraManager& manager ) override = delete;
        ICameraManager& operator=( ICameraManager&& manager ) override      = delete;

        void Initialize() override;
        void Cleanup() override;

        [[nodiscard]] bool IsThisCameraActive( const BalbinoComponent::CCameraComponent* pComponent ) noexcept override;
        [[nodiscard]] const BalbinoComponent::CCameraComponent* GetActiveCamera() const noexcept override;
        [[nodiscard]] BalbinoComponent::CCameraComponent* GetActiveCamera() noexcept override;

        void AddCamera( BalbinoComponent::CCameraComponent* pComponent ) noexcept override;
        void SwitchCameraToActive( BalbinoComponent::CCameraComponent* pComponent ) noexcept override;
        void RemoveCamera( BalbinoComponent::CCameraComponent* pComponent ) noexcept override;

    private:
        std::vector<BalbinoComponent::CCameraComponent*> m_cameras {};
        BalbinoComponent::CCameraComponent* m_pActive = nullptr;
    };
}// namespace Balbino
