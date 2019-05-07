#include "LevelEditor.h"
#include <GLFW/glfw3.h>


#include "../ProjectGE/Level.h"
#include "../ProjectGE/DropDown.h"
#include "../ProjectGE/Button.h"
#include "../ProjectGE/WindowAlignment.h"
#include "../ProjectGE/MouseDrag.h"

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
	Window& window = engine.getGraphics().getWindow();
	window.setBackgroundColor(glm::vec3(50, 50, 50));

	// Create Level Editor
	LevelPtr level = engine.createLevel();
	unsigned char editorLayer = engine.getGraphics().createLayer();
	engine.setCurrentLevel(level);

	// ------------------------------------------------------Menu bar----------------------------------------------------------
	EntityHandle menuBar = level->createEntity("Menu_Bar",
		Rect(1920, 20, { 60, 60, 60, 255 }, editorLayer),
		Transform(0, 0, 10.0f, Alignment::TOP_LEFT)
	);
	// Drop menu
	EntityHandle menuFile = level->createEntity("Menu_Item_File",
		DropDown(Text("File", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255), editorLayer), 50, 20),
		Transform(26, 10, 10.1f, Alignment::CENTER)
	);
	DropDown* file = menuFile.getComponent<DropDown>();
	file->boxPaddingX = 5;
	file->boxPaddingY = 5;
	file->optionHeight = 25;
	file->boxWidth = 300;
	file->addOption("New", Core::bind(file, &DropDown::test));
	file->addOption("Open", Core::bind(file, &DropDown::test));
	file->addOption("Save", Core::bind(file, &DropDown::test));
	file->addOption("Build", Core::bind(file, &DropDown::test));
	// Drop menu
	EntityHandle menuEdit = level->createEntity("Menu_Item_Edit",
		DropDown(Text("Edit", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255), editorLayer), 50, 20),
		Transform(77, 10, 10.1f, Alignment::CENTER)
	);
	file = menuEdit.getComponent<DropDown>();
	file->boxPaddingX = 5;
	file->boxPaddingY = 5;
	file->optionHeight = 25;
	file->boxWidth = 300;
	file->addOption("Undo", Core::bind(file, &DropDown::test));
	file->addOption("Redo", Core::bind(file, &DropDown::test));
	// Drop menu
	EntityHandle menuView = level->createEntity("Menu_Item_View",
		DropDown(Text("View", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255), editorLayer), 50, 20),
		Transform(128, 10, 10.1f, Alignment::CENTER)
	);
	file = menuView.getComponent<DropDown>();
	file->boxPaddingX = 5;
	file->boxPaddingY = 5;
	file->optionHeight = 25;
	file->boxWidth = 300;
	file->addOption("Test", Core::bind(file, &DropDown::test));
	// Drop menu
	EntityHandle menuObject = level->createEntity("Menu_Item_Object",
		DropDown(Text("Object", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255), editorLayer), 50, 20),
		Transform(179, 10, 10.1f, Alignment::CENTER)
	);
	file = menuObject.getComponent<DropDown>();
	file->boxPaddingX = 5;
	file->boxPaddingY = 5;
	file->optionHeight = 25;
	file->boxWidth = 300;
	file->addOption("Create Empty Object", Core::bind(file, &DropDown::test));

	// ---------------------------------------------------Inspector-----------------------------------------------------------------
	// Right Panel.
	int inspectorWidth = 400;
	int inspectorHeight = 1000;
	EntityHandle rightPanel = level->createEntity("Right_Panel",
		Rect(inspectorWidth, inspectorHeight, Color(60,60,60,255), editorLayer),
		WindowAlignment(Alignment::TOP_RIGHT, 0, 20),
		Transform(0, 0, 1.0f, Alignment::TOP_RIGHT)
	);
	int textPadding = 5;
	int backgroundPadding = 3;
	// Inspector label
	EntityHandle inspectorLabel = level->createEntity("Inspector_label",
		Text("Inspector", "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255, 255), editorLayer),
		Transform(textPadding+backgroundPadding-inspectorWidth, textPadding+backgroundPadding, 1.1f, Alignment::TOP_LEFT)
	);
	inspectorLabel.setParent(rightPanel.getEntity());
	// Inspector label background
	Text* inspectorLabelText = inspectorLabel.getComponent<Text>();
	const glm::ivec2& inspectorLabelTextSize = inspectorLabelText->getSize();
	int labelRectWidth = inspectorLabelTextSize.x + textPadding * 2;
	int labelRectHeight = inspectorLabelTextSize.y + textPadding * 2;
	EntityHandle inspectorLabelRect = level->createEntity("Inspector_label_background",
		Rect(labelRectWidth, labelRectHeight, Color(100,100,100,255), editorLayer),
		Transform((float)-textPadding, (float)-textPadding, 1.05f, Alignment::TOP_LEFT)
	);
	inspectorLabelRect.setParent(inspectorLabel.getEntity());

	// Inspector background
	EntityHandle inspectorBackground = level->createEntity("Inspector_background",
		Rect(inspectorWidth - backgroundPadding*2, inspectorHeight - backgroundPadding*2 - labelRectHeight, Color(100,100,100,255), editorLayer),
		Transform(0, labelRectHeight, 1.05f, Alignment::TOP_LEFT)
	);
	inspectorBackground.setParent(inspectorLabelRect.getEntity());

	//------------------------------------------------------GAME---------------------------------------------------------------------
	// Button
	EntityHandle button = level->createEntity("Test_Button",
		Image("resources/images/invaders.png", 0, 300, 300),
		Button("resources/images/invaders.png", "resources/images/gubbe.bmp", "resources/images/awesomeface.png", 300, 300),
		MouseDrag(),
		Border(300, 300, 1, Color(255,255,255,255), false, editorLayer),
		Transform(250, 250, 0.0f, Alignment::CENTER)
	);

	// Level created, calling awake
	level->awake();

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