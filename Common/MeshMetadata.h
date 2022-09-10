#pragma once
#include <vector>
#include <utility>
#include <glm/glm.hpp>

namespace Balbino
{
    struct SSphere
    {
        float radius{ 1.0f };
        glm::vec3 center{};
    };
    struct SBox
    {
        glm::vec3 min{};
        glm::vec3 max{};
    };
	struct SMeshMetadata
	{
        uint32_t firstIndex{};
        uint32_t indexCount{};
        SSphere boundingSphere{};
        SBox boundingBox{};
        SMeshMetadata* pNext{ nullptr};
        SMeshMetadata* pPrevious{ nullptr};
        
        SMeshMetadata operator[](int index)
        {
            if(index == 0) return *this;
            if(!pNext) return {};
            return pNext[index-1];
        }
	};
}
