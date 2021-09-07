#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include "glm.hpp"

namespace Balbino
{
	struct SUniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};

}
