#pragma once
#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4324)
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm.hpp>
#define LIGHT_COUNT 6

struct SModelObject
{
	alignas( 16 ) glm::mat4 model;
	alignas( 16 ) glm::mat4 view;
	alignas( 16 ) glm::mat4 proj;
};

struct SLight
{
	glm::vec4 position;
	glm::vec3 color;
	float radius;
};

struct SLightObject
{
	SLight lights[LIGHT_COUNT];
	glm::vec4 viewPos;
	int displayDebugTarget;
};
#pragma warning(pop)
