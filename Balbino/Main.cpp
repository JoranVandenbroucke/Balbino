#include "BalbinoPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "Balbino.h"

int main(int, char*[]) {
	Balbino::Balbino engine;
	engine.Run();
    return 0;
}