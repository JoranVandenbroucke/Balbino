#pragma once
#include <Core.h>
#include <UUID.h>

namespace BalbinoScene
{
    class CEntity;
}

namespace BalbinoComponent
{
    class CIDComponent final
    {
    public:
        CIDComponent() = default;
        explicit CIDComponent( const CUuid& id ) noexcept
            : m_id { id }
        {
        }

        constexpr ~CIDComponent()                      = default;
        constexpr CIDComponent( const CIDComponent& )  = default;
        constexpr CIDComponent( CIDComponent&& )       = default;
        CIDComponent& operator=( const CIDComponent& ) = default;
        CIDComponent& operator=( CIDComponent&& )      = default;

        inline bool operator==( const CIDComponent& rhs ) const
        {
            return m_id == rhs.m_id;
        }
        inline bool operator!=( const CIDComponent& rhs ) const
        {
            return !( rhs == *this );
        }

        [[nodiscard]] constexpr const CUuid& GetUUID() const noexcept
        {
            return m_id;
        }

    private:
        CUuid m_id {};
    };
}// namespace BalbinoComponent
