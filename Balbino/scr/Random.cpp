//
// Created by Joran Vandenbroucke on 01/06/2022.
//
#include "Random.h"

Balbino::CRandom::CRandom(int seed)
        : m_randomEngine( seed )
          , m_uniformIntDistribution( 0.f, 1.f )
{
}

glm::vec2 Balbino::CRandom::InsideUnitCircle()
{
    glm::vec2 point{};
    do
    {
        point.x = Value();
        point.y = Value();
    }
    while( point.x * point.x + point.y * point.y > 1.0f );
    return point;
}

glm::vec3 Balbino::CRandom::InsideUnitSphere()
{
    glm::vec3 point{};
    do
    {
        point.x = Value();
        point.y = Value();
        point.z = Value();
    }
    while( point.x * point.x + point.y * point.y + point.z * point.z > 1.0f );
    return point;
}

glm::vec3 Balbino::CRandom::OnUnitSphere()
{
    /*
     * x=cosλcosϕ
     * y=cosλsinϕ
     * z=sinλ
     */
    float a = Value() * 3.14159265359f * 2.f;
    float b = Value() * 3.14159265359f * 2.f;
    return glm::vec3{ cos( b ) * cos( a ), cos( b ) * sin( a ), sin( b ) };
}

glm::quat Balbino::CRandom::Rotation()
{
    float x, y, z, u, v, w, s;
    do
    {
        x = Value() * 2 - 1;
        y = Value() * 2 - 1;
        z = x * x + y * y;
    }
    while( z > 1 );
    do
    {
        u = Value() * 2 - 1;
        v = Value() * 2 - 1;
        w = u * u + v * v;
    }
    while( w > 1 );
    s = sqrt(( 1 - z ) / w );
    return glm::quat{ x, y, s * u, s * v };
}

float Balbino::CRandom::Value()
{
    return m_uniformIntDistribution( m_randomEngine );
}
