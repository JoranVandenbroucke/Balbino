#pragma once
#include <utility>
#include <vector>
#include <BambiMath.h>

struct SSphere final {
    BambiMath::Vector3 center{};
    float     radius{ 1.0f };
};
struct SBox final {
    BambiMath::Vector3 min{};
    BambiMath::Vector3 max{};
};

struct SMeshMetadata
{
    uint32_t materialIndex{};
    uint32_t firstIndex{};
    uint32_t indexCount{};
    SSphere  boundingSphere{};
    SBox     boundingBox{};
};

