#pragma once
#include <cstdint>

namespace Balbino
{
	struct SMeshMetadata
	{
		uint32_t materialId;
		uint32_t indexCount;
		uint32_t indexStart;
	};
}
