//
// Created by joran on 05/08/2023.
//

#pragma once
#include <bit>
#include <cmath>
#include <numbers>

namespace BambiMath
{
    constexpr int32_t FloatAsInt( float a )
    {
        return std::bit_cast<int32_t>( a );
    }
    constexpr float IntAsFloat( int32_t a )
    {
        return std::bit_cast<float>( a );
    }

    constexpr float e { std::numbers::e_v<float> };
    constexpr float log2e { std::numbers::log2e_v<float> };
    constexpr float log10e { std::numbers::log10e_v<float> };
    constexpr float pi { std::numbers::pi_v<float> };
    constexpr float invPi { std::numbers::inv_pi_v<float> };
    constexpr float ln2 { std::numbers::ln2_v<float> };
    constexpr float ln10 { std::numbers::ln10_v<float> };
    constexpr float sqrt2 { std::numbers::sqrt2_v<float> };
    constexpr float sqrt3 { std::numbers::sqrt3_v<float> };
    constexpr float invSqrt3 { std::numbers::inv_sqrt3_v<float> };
    constexpr float eGamma { std::numbers::egamma_v<float> };
    constexpr float phi { std::numbers::phi_v<float> };

    constexpr float pi2 { pi * 2 };
    constexpr float infinity { std::numeric_limits<float>::infinity() };
    constexpr float negativeInfinity { -infinity };
    constexpr float epsilon { std::numeric_limits<float>::epsilon() };

    constexpr float radianceToDegree { pi / 180.0f };
    constexpr float degreeToRadiance { 180.0f / pi };

    constexpr float Sqr( float a )
    {
        return a * a;
    }
    constexpr float Qube( float a )
    {
        return a * a * a;
    }
    constexpr float Quartic( float a )
    {
        return Qube( a ) * a;
    }
    constexpr float Quintic( float a )
    {
        return Quartic( a ) * a;
    }
    constexpr float Sextic( float a )
    {
        return Quintic( a ) * a;
    }
    constexpr float Septic( float a )
    {
        return Sextic( a ) * a;
    }
    constexpr float Octic( float a )
    {
        return Septic( a ) * a;
    }
    constexpr float Nonic( float a )
    {
        return Octic( a ) * a;
    }
    constexpr float Decic( float a )
    {
        return Nonic( a ) * a;
    }

    constexpr float e2 { Sqr( e ) };
    constexpr float e3 { Qube( e ) };
    constexpr float e4 { Quartic( e ) };
    constexpr float e5 { Quintic( e ) };
    constexpr float e6 { Sextic( e ) };
    constexpr float e7 { Septic( e ) };
    constexpr float e8 { Octic( e ) };
    constexpr float e9 { Nonic( e ) };
    constexpr float e10 { Decic( e ) };

    // constexpr Quake inverse square root
    constexpr float RSqrt( float a )
    {
        static_assert( std::numeric_limits<float>::is_iec559 );// (enable only on IEEE 754)

        const auto y = std::bit_cast<float>( 0x5f3759df - ( std::bit_cast<std::uint32_t>( a ) >> 1 ) );
        return y * ( 1.5f - ( a * 0.5f * y * y ) );
    }

    constexpr float Abs( float a )
    {
        return std::bit_cast<float>( std::bit_cast<uint32_t>( a ) & ~std::bit_cast<uint32_t>( -0.0f ) );
    }
    constexpr uint64_t Abs( int64_t a )
    {
        uint64_t temp = a >> 63;// make a mask of the sign bit
        temp ^= a;              // toggle the bits if value is negative
        return temp + a & 1;    // add one if value was negative
    }
    constexpr uint32_t Abs( int32_t a )
    {
        uint32_t temp = a >> 31;// make a mask of the sign bit
        temp ^= a;              // toggle the bits if value is negative
        return temp + a & 1;    // add one if value was negative
    }
    constexpr uint16_t Abs( int16_t a )
    {
        uint16_t temp = a >> 15;// make a mask of the sign bit
        temp ^= a;              // toggle the bits if value is negative
        return temp + a & 1;    // add one if value was negative
    }
    constexpr uint8_t Abs( int8_t a )
    {
        uint8_t temp = a >> 7;// make a mask of the sign bit
        temp ^= a;            // toggle the bits if value is negative
        return temp + a & 1;  // add one if value was negative
    }
    template<typename T>
    constexpr T Abs( const T x ) noexcept
    {
        return x == T( 0 ) ? T( 0 ) : x < T( 0 ) ? -x : x;
    }

    inline float Sine( float a )
    {
        return std::sinf( a );
    }
    inline float Cosine( float a )
    {
        return std::cosf( a );
    }
    inline float Tangent( float a )
    {
        return std::tanf( a );
    }
    inline float ArkSine( float a )
    {
        return std::asinf( a );
    }
    inline float ArkCosine( float a )
    {
        return std::acosf( a );
    }
    inline float ArkTangent( float a )
    {
        return std::atanf( a );
    }
    inline float ArkTangent2( float y, float x )
    {
        return std::atan2f( y, x );
    }
    inline float HyperbolicSine( float a )
    {
        return std::sinhf( a );
    }
    inline float HyperbolicCosine( float a )
    {
        return std::cosf( a );
    }
    inline float HyperbolicTangent( float a )
    {
        return std::tanf( a );
    }
    inline float HyperbolicArkSine( float a )
    {
        return std::asinhf( a );
    }
    inline float HyperbolicArkCosine( float a )
    {
        return std::acoshf( a );
    }
    inline float HyperbolicArkTangent( float a )
    {
        return std::atanhf( a );
    }

    inline float Exponent( float a )
    {
        return std::expf( a );
    }
    inline float Logarithm( float a )
    {
        return std::logf( a );
    }
    inline float Logarithm10( float a )
    {
        return std::log10f( a );
    }
    inline float Log2( float a )
    {
        return std::logf( a );
    }
    constexpr float Ceil( float a )
    {
        return (float)( a < 0.0f ? int( a ) : int( a ) + 1 );
    }
    constexpr float Floor( float a )
    {
        return (float)( a < 0.0f ? int( a ) - 1 : int( a ) );
    }
    constexpr float Round( float a )
    {
        return (float)(int)( a + 0.5f );
    }
    constexpr int CeilToInt( float a )
    {
        return (int)Ceil( a );
    }
    constexpr int FloorToInt( float a )
    {
        return (int)Floor( a );
    }
    constexpr int RoundToInt( float a )
    {
        return (int)Round( a );
    }
    constexpr float Modulo( float a, float b )
    {
        return a - b * Floor( a / b );
    }

    constexpr float Max( float a, float b )
    {
        return std::max( a, b );
    }
    constexpr float Min( float a, float b )
    {
        return std::min( a, b );
    }
    constexpr int Max( int a, int b )
    {
        return std::max( a, b );
    }
    constexpr int Min( int a, int b )
    {
        return std::min( a, b );
    }
    constexpr int Sign( float a )
    {
        return ( 0.0f < a ) - ( a < 0.0f );
    }
    constexpr int Sign( int a )
    {
        return ( 0 < a ) - ( a < 0 );
    }
    constexpr float Clamp( float value, float min, float max )
    {
        return Min( Max( value, min ), max );
    }
    constexpr float Clamp01( float value )
    {
        return Clamp( value, 0, 1 );
    }
    constexpr int ClosestPowerOfTwo( int a )
    {
        --a;
        a |= a >> 1;
        a |= a >> 2;
        a |= a >> 4;
        a |= a >> 8;
        a |= a >> 16;
        ++a;
        return a;
    }
    constexpr float Lerp( float a, float b, float t )
    {
        return a + t * ( b - a );
    }
    constexpr float InverseLerp( float a, float b, float v )
    {
        return ( v - a ) / ( b - a );
    }
    constexpr float Remap( float inMin, float inMax, float outMin, float outMax, float value )
    {
        return Lerp( outMin, outMax, InverseLerp( inMin, inMax, value ) );
    }
    constexpr float LerpClamped( float a, float b, float t )
    {
        return Clamp( Lerp( a, b, t ), a, b );
    }
    constexpr float InverseLerpClamped( float a, float b, float v )
    {
        return Clamp01( InverseLerp( a, b, v ) );
    }
    constexpr float RemapClamped( float inMin, float inMax, float outMin, float outMax, float value )
    {
        return Lerp( outMin, outMax, InverseLerpClamped( inMin, inMax, value ) );
    }
    constexpr float LerpAngle( float a, float b, float t )
    {
        a = Modulo( a, pi2 );
        b = Modulo( b, pi2 );
        return a + t * ( Modulo( b - a + 3 * pi, pi2 ) - pi );
    }
    constexpr float SMin( float min, float max, float smooth )
    {
        float h = Clamp01( 0.5f + 0.5f * ( min - max ) / smooth );
        return Lerp( min, max, h ) - smooth * h * ( 1.0f - h );
    }
    inline float SMax( float a, float b, float v )
    {
        return a + Logarithm( 1 + Exponent( v * ( b - a ) ) ) / v;
    }
}// namespace BambiMath