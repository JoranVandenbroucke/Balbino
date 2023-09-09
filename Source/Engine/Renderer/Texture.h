#pragma once
#include "FawnVisionCore.h"
#include <cstdint>

#include "UUID.h"

namespace FawnVision
{
    class CTexture final
    {
    public:
        explicit CTexture( FawnVision::Device device, CUuid uuid );
        constexpr ~CTexture()
        {
            m_device = BALBINO_NULL;
        }
        constexpr CTexture( const CTexture& )            = delete;
        constexpr CTexture( CTexture&& )                 = delete;
        constexpr CTexture& operator=( const CTexture& ) = delete;
        constexpr CTexture& operator=( CTexture&& )      = delete;

        void Initialize( const FawnVision::STextureCreateInfo& createInfo, FawnVision::CommandPool pool, FawnVision::Queue queue, bool generateMips ) noexcept;
        void Cleanup() const;

        [[nodiscard]] constexpr FawnVision::ImageView GetImageViewObject() const
        {
            return m_view;
        }
        [[nodiscard]] constexpr FawnVision::Sampler GetSamplerObject() const
        {
            return m_sampler;
        }
        inline const CUuid& GetId() const noexcept
        {
            return m_uuid;
        }
        inline CUuid GetId() noexcept
        {
            return m_uuid;
        }

    private:
        const CUuid m_uuid {};

        FawnVision::ImageView m_view { BALBINO_NULL };
        FawnVision::ImageResource m_resource { BALBINO_NULL };
        FawnVision::Sampler m_sampler { BALBINO_NULL };
        FawnVision::Device m_device { BALBINO_NULL };
    };
}// namespace FawnVision