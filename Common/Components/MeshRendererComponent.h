#pragma once
#include "../IEntity.h"

class CMeshRenderComponent
{
public:
    CMeshRenderComponent() = default;
    explicit CMeshRenderComponent( IEntity* pEntity, const CUuid& meshId )
            : m_pEntity{ pEntity }
              , m_meshId{ meshId }
    {
    }
    
    ~CMeshRenderComponent() = default;
    CMeshRenderComponent( const CMeshRenderComponent& ) = default;
    CMeshRenderComponent( CMeshRenderComponent&& ) = default;
    CMeshRenderComponent& operator=( const CMeshRenderComponent& ) = default;
    CMeshRenderComponent& operator=( CMeshRenderComponent&& ) = default;
    
    [[nodiscard]] const CUuid& GetMeshId() const
    {
        return m_meshId;
    }
    void SetMaterial( uint32_t idx, CUuid materialId )
    {
        if ( idx < m_materielUuid.size())
        {
            m_materielUuid[idx] = materialId;
        }
    }
    void SetMaterialCount( uint64_t size )
    {
        m_materielUuid.resize( size );
    }
    [[nodiscard]] const std::vector<CUuid>& GetMaterials() const
    {
        return m_materielUuid;
    }
    bool operator==( const CMeshRenderComponent& other ) const
    {
        return this->m_meshId == other.m_meshId;
    }
private:
    IEntity* m_pEntity{};
    CUuid              m_meshId;
    std::vector<CUuid> m_materielUuid;
};
