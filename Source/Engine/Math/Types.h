//
// Created by joran on 05/08/2023.
//

#pragma once
#pragma warning( push )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4723 )
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace BambiMath
{
    using Vector1    = glm::vec1;
    using Vector2    = glm::vec2;
    using Vector3    = glm::vec3;
    using Vector4    = glm::vec4;
    using IVector1   = glm::ivec1;
    using IVector2   = glm::ivec2;
    using IVector3   = glm::ivec3;
    using IVector4   = glm::ivec4;
    using Matrix2    = glm::mat2;
    using Matrix2x2  = glm::mat2x2;
    using Matrix2x3  = glm::mat2x3;
    using Matrix2x4  = glm::mat2x4;
    using Matrix3    = glm::mat3;
    using Matrix3x2  = glm::mat3x2;
    using Matrix3x3  = glm::mat3x3;
    using Matrix3x4  = glm::mat3x4;
    using Matrix4    = glm::mat4;
    using Matrix4x2  = glm::mat4x2;
    using Matrix4x3  = glm::mat4x3;
    using Matrix4x4  = glm::mat4x4;
    using Quaternion = glm::quat;

    constexpr ImVec2 Vec2Converter( const glm::vec2& v )
    {
        return { v.x, v.y };
    }
    constexpr ImVec4 Vec2Converter( const glm::vec4& v )
    {
        return { v.x, v.y, v.z, v.w };
    }

    constexpr Vector1 Abs( const Vector1& v )
    {
        return glm::abs( v );
    }
    constexpr Vector2 Abs( const Vector2& v )
    {
        return glm::abs( v );
    }
    constexpr Vector3 Abs( const Vector3& v )
    {
        return glm::abs( v );
    }
    constexpr Vector4 Abs( const Vector4& v )
    {
        return glm::abs( v );
    }
    constexpr Vector3 Rotate( Quaternion q, Vector3 p )
    {
        return q * p;
    }
    inline Matrix4 PerspectiveMatrix( float fov, float aspectRatio, float nearPlane, float farPlane ) noexcept
    {
        return glm::perspective( fov, aspectRatio, nearPlane, farPlane );
    }
    inline Matrix4 OrthographicMatrix( float fov, float aspectRatio, float nearPlane, float farPlane ) noexcept
    {
        return glm::ortho( fov, aspectRatio, nearPlane, farPlane );
    }
    inline Matrix4 LookAt( const Vector3& pos, const Vector3& target, const Vector3& up )
    {
        return glm::lookAt( pos, target, up );
    }
    inline Matrix4 Transpose( const Matrix4& m )
    {
        return glm::transpose( m );
    }
    inline float Distance( const Vector3& p1, const Vector3& p2 )
    {
        return glm::distance( p1, p2 );
    }
    inline Vector3 EulerAngles( const Quaternion& q )
    {
        return glm::eulerAngles( q );
    }
#pragma warning( pop )
}// namespace BambiMath