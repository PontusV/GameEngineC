#include "ParentEntity.h"
#include "Panel.h"

#include "Engine.h"
#include "Level.h"
#include <iostream>
#include <sstream>

using namespace Core;

void test() {
	std::cout << "Button press!\n";
}

int main(int argc, char *argv[])
{
	Engine engine;
	//LevelPtr level = engine.createLevel();
	//engine.setCurrentLevel(level);

	LevelPtr level = engine.loadLevel("level.dat");
	engine.setCurrentLevel(level);

	/*EntityHandle parent = level->createEntity("Test_Button",
		new Image("resources/images/invaders.png", 0, 300, 300),
		new Text("Rectangle Text", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		new Button("resources/images/invaders.png", "resources/images/gubbe.bmp", "resources/images/awesomeface.png", 300, 300),
		new Panel(800, 800),
		new Transform(250, 250, 1.0f, TransformAnchor::CENTER)
	);

	level->createEntity("Box",
		new Rect(300, 50, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),
		new Text("Rectangle Text", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f,1.0f,1.0f,1.0f)),
		new Transform(500, 500, 0, TransformAnchor::CENTER)
	);

	EntityHandle child = level->createEntity("Test_Text",
		new Image("resources/images/invaders.png", 0, 300, 300),
		new Text("This is just a test text!", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		new Transform(500, 300, 1.0f, TransformAnchor::TOP_LEFT)
	);
	
	child.addComponent<ParentEntity>(parent.getEntity());

	engine.saveLevel("level.dat"); //*/

	/*for (int i = 0; i < 650; i++) {
		std::stringstream ss;
		ss << "Spam_Test" << i;
		level->createEntity(ss.str(),
			new Image("resources/images/invaders.png", 0, 300, 300),
			new Text("Rectangle Text", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			new Transform(250, 250, 1.0f, TransformAnchor::CENTER)
		);
	}*/

	EntityHandle entity = level->getEntity("Test_Button");
	Button* button = entity.getComponent<Button>();
	if (button) button->setButtonPressCallback(test);

	int exit = engine.initiate();
	return exit;
}