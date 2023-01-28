#pragma once
#include <glm/glm.hpp>

#include <cmath>

namespace Balbino
{
    class CTexture;
    
    class CMaterial;
    
    class CCamera
    {
        enum class EClearMode : uint8_t
        {
            Inheret,
            None,
            Color,
            Shader,
            Image
        };
        union UClear
        {
            float color[4];
            CMaterial* MaterialId;
            CTexture * TextureId;
        };
    public:
        CCamera();
        
        ~CCamera() = default;
        CCamera( const CCamera& other ) = default;
        
        CCamera( CCamera&& ) = default;
        CCamera& operator=( const CCamera& ) = default;
        CCamera& operator=( CCamera&& ) = default;
        
        void UpdateFrameBuffer( float width, float height, float fov = glm::radians( 45.0f ), float nearPlane = 0.001f, float farPlane = 1000 );
        void UpdateMatrices( const glm::vec3& position, const glm::quat& rotation );
        [[nodiscard]] const glm::mat4& GetProjection() const;
        [[nodiscard]] const glm::mat4& GetView() const;
        [[nodiscard]] const glm::vec4& GetViewPosition() const;
        [[nodiscard]] float GetFov() const;
        [[nodiscard]] float GetWidth() const;
        [[nodiscard]] float GetHeight() const;
    
    private:
        
        glm::mat4 m_projMatrix;
        glm::mat4 m_viewMatrix;
        glm::vec4 m_viewPosition;
        
        float m_fov;
        float m_width;
        float m_height;
        
    };
}
