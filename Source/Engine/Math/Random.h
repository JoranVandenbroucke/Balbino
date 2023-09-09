//
// Created by Joran Vandenbroucke on 01/06/2022.
//

#pragma once
#include <random>

#include "Types.h"

namespace BambiMath
{
    class CRandom final
    {
    public:
        explicit CRandom( int seed = 0 ) noexcept;
        ~CRandom()                           = default;
        CRandom( const CRandom& )            = delete;
        CRandom( CRandom&& )                 = delete;
        CRandom& operator=( CRandom&& )      = delete;
        CRandom& operator=( const CRandom& ) = delete;

        [[nodiscard]] inline Vector2 InsideUnitCircle() noexcept;
        [[nodiscard]] inline Vector3 InsideUnitSphere() noexcept;
        [[nodiscard]] inline Vector3 OnUnitSphere() noexcept;
        [[nodiscard]] inline Quaternion Rotation() noexcept;
        [[nodiscard]] inline float Value() noexcept;

    private:
        std::mt19937 m_randomEngine;
        std::uniform_real_distribution<float> m_uniformIntDistribution { 0.f, 1.f };
    };
}// namespace Balbino
