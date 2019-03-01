#include "Engine.h"

#include "Level.h"
using namespace GameEngine;

int main(int argc, char *argv[])
{
	Engine engine;
	LevelPtr level = engine.createLevel();
	level->createEntity("Test",
		new Image("resources/images/invaders.png", 0, 0),
		new Transform(100,100)
	);
	level->createEntity("Test_Text",
		new Text("Test", "resources/fonts/cambriab.ttf", 28, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		new Transform(500, 100)
	);

	int exit = engine.initiate();
	return exit;
}