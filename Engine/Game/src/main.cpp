#include <Core/Camera.h>
#include <Core/Component.h>
#include <Core/Core.h>
#include <Core/Engine.h>
#include <Core/Entity.h>
#include <Core/EntityHandle.h>
#include <Core/EntityManager.h>
#include <Core/Graphics.h>
#include <Core/HideFlags.h>
#include <Core/Input.h>
#include <Core/Scene.h>
#include <Core/SceneManager.h>
#include <Core/Window.h>
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
	// TODO: Load scene
	return engine.start();
}