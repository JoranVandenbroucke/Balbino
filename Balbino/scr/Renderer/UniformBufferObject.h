#pragma once
#include "glm.hpp"

namespace Balbino
{
	struct SUniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	};

}
