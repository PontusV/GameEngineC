#pragma comment(linker, "/export:createDLLInterface")
#pragma comment(linker, "/export:releaseDLLInterface")
#include "engine/Engine.h"
#include <cstddef>
using namespace Core;

/* Used by EXE */
int main(int argc, char* argv[]) {
	Engine engine;
	std::size_t init = engine.initiate();
	if (init != 0) return init;
	return engine.start();
}