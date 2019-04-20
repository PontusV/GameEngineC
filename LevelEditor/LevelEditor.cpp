#include "LevelEditor.h"
#include <GLFW/glfw3.h>


#include "../ProjectGE/Level.h"
//#include <sstream> // For testing massive amounts of Entities

using namespace Core;

LevelEditor::LevelEditor() {
	engine.getInput().addKeyListener(this);
}


LevelEditor::~LevelEditor() {
}


void buttonPressTest() {
	std::cout << "Button press!\n";
}

int LevelEditor::initiate() {
	// Initiate engine
	int result = engine.initiate();
	if (result != 0) {
		return result;
	}
	engine.getGraphics().getWindow().setBackgroundColor(glm::vec3(0.2f, 0.2f, 0.2f));

	// Create Level Editor
	glm::vec4 themeColor = glm::vec4(0.25f,0.25f,0.25f,1.0f);
	LevelPtr level = engine.createLevel();
	unsigned char layer = engine.getGraphics().createLayer();

	engine.setCurrentLevel(level);
	EntityHandle menuBar = level->createEntity("Menu_Bar",
		Rect(1080, 20, themeColor, layer),
		Panel(1080, 20),
		Transform(0, 0, 0.0f, TransformAnchor::TOP_LEFT)
	);
	// Drop menu
	EntityHandle menuFile = level->createEntity("Menu_Item_File",
		Text("File", "resources/fonts/segoeui.ttf", 14, glm::vec4(1.0f,1.0f,1.0f,1.0f), layer),
		Transform(15, 10, 0.0f, TransformAnchor::CENTER)
	);
	// Drop menu
	EntityHandle menuEdit = level->createEntity("Menu_Item_Edit",
		Text("Edit", "resources/fonts/segoeui.ttf", 14, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), layer),
		Transform(50, 10, 0.0f, TransformAnchor::CENTER)
	);
	// Drop menu
	EntityHandle menuCreate = level->createEntity("Menu_Item_Create",
		Text("Create", "resources/fonts/segoeui.ttf", 14, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), layer),
		Transform(95, 10, 0.0f, TransformAnchor::CENTER)
	);

	/*for (std::size_t i = 0; i < 9500; i++) {
		std::stringstream ss;
		ss << "Test" << i;
		EntityHandle menuCreate = level->createEntity(ss.str(),
			Image("resources/images/invaders.png", 0, 300, 300),
			Transform(0+i, 550, 0.1f, TransformAnchor::CENTER)
		);
	}*/

	// Button
	EntityHandle button = level->createEntity("Test_Button",
		Image("resources/images/invaders.png", 0, 300, 300),
		Button("resources/images/invaders.png", "resources/images/gubbe.bmp", "resources/images/awesomeface.png", 300, 300),
		Transform(250, 250, 0.1f, TransformAnchor::CENTER)
	);
	menuFile.setParent(menuBar.getEntity());
	menuEdit.setParent(menuBar.getEntity());
	menuCreate.setParent(menuBar.getEntity());


	/*EntityHandle entity = level->getEntity("Test_Button");
	Button* bbutton = entity.getComponent<Button>();
	if (bbutton) bbutton->setButtonPressCallback(buttonPressTest);//*/

	// Create test level
	/*LevelPtr level = engine.createLevel();
	engine.setCurrentLevel(level);

	EntityHandle parent = level->createEntity("Test_Button",
		new Image("resources/images/invaders.png", 0, 300, 300),
		//new Text("Rectangle Text", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		new Button("resources/images/invaders.png", "resources/images/gubbe.bmp", "resources/images/awesomeface.png", 300, 300),
		new Panel(800, 800),
		new Transform(250, 250, 0.1f, TransformAnchor::CENTER)
	);

	EntityHandle test = level->createEntity("Box",
		new Rect(300, 50, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),
		new Border(300, 50, 1, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), false),
		new Text("Rectangle Text", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		new Transform(200, 500, 0, TransformAnchor::CENTER)
	);

	EntityHandle child = level->createEntity("Test_Text",
		new Image("resources/images/invaders.png", 0, 300, 300),
		//new Text("This is just a test text!", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		new Transform(200, 200, 0.1f, TransformAnchor::TOP_LEFT)
	);

	child.setParent(parent.getEntity());
	child.addComponent<Text>("This is just a test text!", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	parent.addComponent<Text>("Rectangle Text", "resources/fonts/cambriab.ttf", 24, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));*/

	// Keybinds
	engine.getInput().addKeyBind(GLFW_KEY_ESCAPE, "Terminate");
	engine.getInput().addKeyBind(GLFW_KEY_F5, "Save");
	engine.getInput().addKeyBind(GLFW_KEY_F9, "Load");

	// Window hints
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	//Start game loop
	return engine.start();
}

void LevelEditor::keyPressed(std::string buttonName) {
	if (buttonName == "Save") {
		engine.saveLevel("level.dat");
	}
	if (buttonName == "Load") {
		LevelPtr loadedLevel = engine.loadLevel("level.dat");
		engine.setCurrentLevel(loadedLevel);
	}
}
void LevelEditor::keyReleased(std::string buttonName) {
	if (buttonName == "Terminate")
		engine.terminate();
}