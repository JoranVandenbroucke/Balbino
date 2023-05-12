#pragma once
#include <cmath>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Balbino::BalMath
{
    inline bool DecomposeTransform( const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale )
    {
        // From glm::decompose in matrix_decompose.inl
        
        glm::mat4 localMatrix( transform );
        
        // Normalize the matrix.
        if ( glm::epsilonEqual( localMatrix[3][3], static_cast<float>( 0 ), glm::epsilon<float>()))
        {
            return false;
        }
        
        // First, isolate perspective.  This is the messiest.
        if ( glm::epsilonNotEqual(
                localMatrix[0][3], static_cast<float>( 0 ), glm::epsilon<float>()) || glm::epsilonNotEqual(
                localMatrix[1][3], static_cast<float>( 0 ), glm::epsilon<float>()) || glm::epsilonNotEqual(
                localMatrix[2][3], static_cast<float>( 0 ), glm::epsilon<float>()))
        {
            // Clear the perspective partition
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<float>( 0 );
            localMatrix[3][3] = static_cast<float>( 1 );
        }
        
        // Next take care of translation (easy).
        translation = glm::vec3( localMatrix[3] );
        localMatrix[3] = glm::vec4( 0, 0, 0, localMatrix[3].w );
        
        std::array<glm::vec3, 3> Row;
        
        // Now get scale and shear.
        for ( glm::length_t i{}; i < 3; ++i )
        {
            for ( glm::length_t j{}; j < 3; ++j )
            {
                Row[i][j] = localMatrix[i][j];
            }
        }
        
        // Compute X scale factor and normalize first row.
        scale.x = length( Row[0] );
        Row[0] = glm::detail::scale( Row[0], 1.f );
        scale.y = length( Row[1] );
        Row[1] = glm::detail::scale( Row[1], 1.f );
        scale.z = length( Row[2] );
        Row[2] = glm::detail::scale( Row[2], 1.f );
        
        // At this point, the matrix (in rows[]) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.
        #if 0
        glm::vec3 Pdum3 = cross( Row[1], Row[2] ); // v3Cross(row[1], row[2], Pdum3);
        if ( dot( Row[0], Pdum3 ) < 0 )
        {
            for ( glm::length_t i = 0; i < 3; i++ )
            {
                scale[i] *= static_cast< float >( -1 );
                Row[i] *= static_cast< float >( -1 );
            }
        }
        #endif
        
        rotation.y = asin( -Row[0][2] );
        if ( cos( rotation.y ) != 0 )
        {
            rotation.x = atan2( Row[1][2], Row[2][2] );
            rotation.z = atan2( Row[0][1], Row[0][0] );
        }
        else
        {
            rotation.x = atan2( -Row[2][0], Row[1][1] );
            rotation.z = 0;
        }
        
        
        return true;
    }
}
