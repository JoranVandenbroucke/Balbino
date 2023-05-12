//
// Created by joran on 22/10/2022.
//

#ifndef GAME_CAMERAMANAGER_H
#define GAME_CAMERAMANAGER_H

#include <ICameraManager.h>

namespace Balbino
{
    class CCameraManager final : public ICameraManager
    {
    public:
        CCameraManager() = default;
        ~CCameraManager() override = default;
        CCameraManager( const CCameraManager& ) = delete;
        CCameraManager( CCameraManager&& ) = delete;
        ICameraManager& operator=( const ICameraManager& manager ) override = delete;
        ICameraManager& operator=( ICameraManager&& manager ) override = delete;
        
        void Initialize() override;
        void AddCamera( CCameraComponent* pComponent ) override;
        void SwitchCameraToActive( CCameraComponent* pComponent ) override;
        void RemoveCamera( CCameraComponent* pComponent ) override;
        bool IsThisCameraActive( const CCameraComponent* pComponent ) override;
        [[nodiscard]] const CCameraComponent* GetActiveCamera() const override;
        CCameraComponent* GetActiveCamera() override;
        void Cleanup() override;
    
    private:
        std::vector<CCameraComponent*> m_cameras{};
        CCameraComponent* m_pActive = nullptr;
    };
} // Balbino

#endif //GAME_CAMERAMANAGER_H