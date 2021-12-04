#pragma once
#include <glm.hpp>

#define LIGHT_COUNT 6
struct  SModelObject{
	glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
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