#include "pch.h"
#include <Balbino.h>

class BubbleBobble: public Balbino::Application
{
public:
	BubbleBobble() = default;
};

Balbino::Application* Balbino::CreateApplication()
{
	return new BubbleBobble{};
}