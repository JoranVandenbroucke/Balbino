//
// Created by Joran Vandenbroucke on 01/06/2022.
//

#pragma once
#include <random>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Balbino
{
    class CRandom
    {
    public:
        explicit CRandom( int seed = 0 );
        ~CRandom() = default;
        CRandom( const CRandom& ) = delete;
        CRandom( CRandom&& ) = delete;
        CRandom& operator=( CRandom&& ) = delete;
        CRandom& operator=( const CRandom& ) = delete;
        
        glm::vec2 InsideUnitCircle();
        glm::vec3 InsideUnitSphere();
        glm::vec3 OnUnitSphere();
        glm::quat Rotation();
        float Value();
    private:
        std::mt19937                          m_randomEngine;
        std::uniform_real_distribution<float> m_uniformIntDistribution;
    };
} // Balbino



#pragma warning(pop)
