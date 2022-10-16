#pragma once
#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4324)
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <Light.h>

#define LIGHT_COUNT 8

struct SModelObject
{
    alignas( 16 ) glm::mat4 view;
    alignas( 16 ) glm::mat4 proj;
    alignas( 16 ) glm::vec4 viewPos;
    alignas( 4 ) int        displayDebugTarget;
};

struct SLight
{
    alignas( 4 ) Balbino::ELightType type;      //type: Directional, Point, Spot, Area
    alignas( 4 ) float               strength;                //strength
    alignas( 16 ) glm::vec3          position;           //position
    alignas( 16 ) glm::vec3          direction;          //direction
    alignas( 16 ) glm::vec3          color;              //color
    alignas( 16 ) glm::vec3          size;               //Point: size,0,0; Spot: size, front, back; area: width, height, 0
};

struct SLightObject
{
    SLight lights[LIGHT_COUNT];
};
#pragma warning(pop)
