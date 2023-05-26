#pragma once
#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4324)
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include "../../3rdParty/GLM/glm/glm.hpp"
#include <Light.h>

#define LIGHT_COUNT 8

struct SModelObject
{
    glm::mat4 view{};
    glm::mat4 proj{};
    glm::vec4 viewPos{};
    int       displayDebugTarget{};
};

struct SLight
{
    SLight() = default;
    SLight( light_type t, float str, const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& col, const glm::vec3& si )
            : SLight{ (int) t, str, pos, dir, col, si }
    {
    }
    SLight( int t, float str, const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& col, const glm::vec3& si )
            : type{ t }
              , strength{ str }
              , position{ pos }
              , direction{ dir }
              , color{ col }
              , size{ si }
    {
    }
    
    int                    type{1};              //type: Directional, Point, Spot, Area
    float                  strength{1};          //strength
    alignas( 16 )glm::vec3 position{0,0,0};          //position
    alignas( 16 )glm::vec3 direction{0,0,0};         //direction
    alignas( 16 )glm::vec3 color{1,1,1};             //color
    alignas( 16 )glm::vec3 size{100,0,0};              //Point: size,0,0; Spot: size, front, back; area: width, height, 0
    
};

struct SLightObject
{
    SLight lights[LIGHT_COUNT];
};
#pragma warning(pop)
