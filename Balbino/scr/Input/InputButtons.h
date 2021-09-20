#pragma once
#include <functional>
namespace Balbino
{
	enum class EInputEvent
	{
		Down = 0,
		Up = 1,
		Repeat = 2,
		DoubleClick = 3,
		Axis = 4,
	};
	struct CInputAxis
	{
		const char* name;
		uint16_t code;
		float weight;
	};
	struct CInputAxisBinding
	{
		float value;
		std::function<void(float)> function;
	};
}