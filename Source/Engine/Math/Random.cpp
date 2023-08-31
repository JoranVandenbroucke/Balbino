//
// Created by Joran Vandenbroucke on 01/06/2022.
//
#include "Random.h"
#include <numbers>

BambiMath::CRandom::CRandom( int seed ) noexcept
    : m_randomEngine( seed )
{
}

inline BambiMath::Vector2 BambiMath::CRandom::InsideUnitCircle() noexcept
{
    Vector2 point {};
    do
    {
        point.x = Value();
        point.y = Value();
    } while ( point.x * point.x + point.y * point.y > 1.0f );
    return point;
}

inline BambiMath::Vector3 BambiMath::CRandom::InsideUnitSphere() noexcept
{
    Vector3 point {};
    do
    {
        point.x = Value();
        point.y = Value();
        point.z = Value();
    } while ( point.x * point.x + point.y * point.y + point.z * point.z > 1.0f );
    return point;
}

inline BambiMath::Vector3 BambiMath::CRandom::OnUnitSphere() noexcept
{
    /*
     * x=cosλcosϕ
     * y=cosλsinϕ
     * z=sinλ
     */
    float a = Value() * std::numbers::pi_v<float> * 2.f;
    float b = Value() * std::numbers::pi_v<float> * 2.f;
    return Vector3 { cos( b ) * cos( a ), cos( b ) * sin( a ), sin( b ) };
}

inline BambiMath::Quaternion BambiMath::CRandom::Rotation() noexcept
{
    float x;
    float y;
    float z;
    float u;
    float v;
    float w;
    float s;
    do
    {
        x = Value() * 2 - 1;
        y = Value() * 2 - 1;
        z = x * x + y * y;
    } while ( z > 1 );
    do
    {
        u = Value() * 2 - 1;
        v = Value() * 2 - 1;
        w = u * u + v * v;
    } while ( w > 1 );
    s = std::sqrt( ( 1 - z ) / w );
    return { x, y, s * u, s * v };
}

inline float BambiMath::CRandom::Value() noexcept
{
    return m_uniformIntDistribution( m_randomEngine );
}
