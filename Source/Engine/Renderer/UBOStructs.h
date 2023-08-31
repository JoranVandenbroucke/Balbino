#pragma once
#include <BambiMath.h>

#define LIGHT_COUNT 8

struct SModelObject final {
    BambiMath::Matrix4 view {};
    BambiMath::Matrix4 proj {};
    BambiMath::Vector4 viewPos {};
};
struct SIndirectBatch final {
    CUuid mesh {};
    CUuid material {};
    uint32_t firstInstance {};
    uint32_t instanceCount {};
    uint32_t firstIndex {};
    uint32_t indexCount {};
};

struct SLight final {
    SLight() = default;
    SLight( uint8_t t, float str, const BambiMath::Vector3& pos, const BambiMath::Vector3& dir, const BambiMath::Vector3& col, const BambiMath::Vector3& si )
        : type { t }
        , position { pos }
        , direction { dir }
        , color { col * str}
        , size { si }
    {
    }
    BambiMath::Vector3 position { 0, 0, 0 }; //position
    BambiMath::Vector3 direction { 0, 0, 0 };//direction
    BambiMath::Vector3 color { 1, 1, 1 };    //color
    BambiMath::Vector3 size { 100, 0, 0 };   //Point: size,0,0; Spot: size, front, back; area: width, height, 0
    uint8_t type { 1 };                                    //type: Directional, Point, Spot, Area
};
struct SLightObject final {
    SLight lights[ LIGHT_COUNT ];
};
