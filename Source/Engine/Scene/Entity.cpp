#include "Entity.h"
#include "Scene.h"

namespace BalbinoScene
{
    bool CEntity::IsValid() const noexcept
    {
        if ( m_registry )
            return m_registry->valid( m_entity );
        return false;
    }
}// namespace BalbinoScene