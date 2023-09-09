#pragma once
#include <Core.h>
#include <UUID.h>

namespace BalbinoComponent
{
class CMeshRenderComponent final
{
public:
    constexpr CMeshRenderComponent() = default;
    constexpr explicit CMeshRenderComponent( const CUuid& meshId ) noexcept
        : m_meshId { meshId }
    {
    }

    constexpr ~CMeshRenderComponent()                              = default;
    constexpr CMeshRenderComponent( const CMeshRenderComponent& )  = default;
    constexpr CMeshRenderComponent( CMeshRenderComponent&& )       = default;
    CMeshRenderComponent& operator=( const CMeshRenderComponent& ) = default;
    CMeshRenderComponent& operator=( CMeshRenderComponent&& )      = default;

    inline bool operator==( const CMeshRenderComponent& other ) const
    {
        return this->m_meshId == other.m_meshId;
    }
    inline bool operator!=( const CMeshRenderComponent& other ) const
    {
        return !( *this == other );
    }

    [[nodiscard]] constexpr const CUuid& GetMeshId() const noexcept
    {
        return m_meshId;
    }
    [[nodiscard]] constexpr const std::vector<CUuid>& GetMaterials() const noexcept
    {
        return m_materielUuid;
    }
    inline void SetMaterial( uint32_t idx, const CUuid& materialId ) noexcept
    {
        if ( idx < m_materielUuid.size() )
        {
            m_materielUuid[ idx ] = materialId;
        }
    }
    inline void SetMaterialCount( uint64_t size ) noexcept
    {
        m_materielUuid.resize( size );
    }

private:
    CUuid m_meshId {};
    std::vector<CUuid> m_materielUuid {};
};
}
