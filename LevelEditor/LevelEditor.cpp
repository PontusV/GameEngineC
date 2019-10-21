#include "LevelEditor.h"
#include <GLFW/glfw3.h>

#include "scene/Scene.h"
#include "components/RectTransform.h"
#include "components/graphics/ui/DropDown.h"
#include "components/graphics/ui/Button.h"
#include "components/graphics/ui/WindowAnchor.h"
#include "components/MouseDrag.h"
#include "components/graphics/ui/LayoutElement.h"
#include "components/graphics/ui/HorizontalLayoutGroup.h"
#include "components/graphics/ui/VerticalLayoutGroup.h"
#include "components/graphics/ui/WindowScale.h"
#include "components/DragAndResize.h"
#include "components/graphics/ui/RectButton.h"
#include "components/editor/EditorPanel.h"

#include "maths/Vector2.h"
#include "maths/Vector3.h"

using namespace Core;

LevelEditor::LevelEditor() {
	engine.getInput().addKeyListener(this);
}


LevelEditor::~LevelEditor() {
}



int LevelEditor::initiate() {
	// Initiate engine
	int result = engine.initiate();
	if (result != 0) {
		return result;
	}
	Window& window = engine.getGraphics().getWindow();
	window.setBackgroundColor(Vector3(0.1f, 0.1f, 0.1f));

	// Create Level Editor
	ScenePtr sceneUI = engine.getSceneManager().createScene("Level_Editor", ObjectType::UI);
	ScenePtr sceneWorld = engine.getSceneManager().createScene("World", ObjectType::World);
	unsigned char editorSortingLayer = 0;
	// ------------------------------------------------------Menu bar----------------------------------------------------------
	EntityHandle menuBar = sceneUI->createEntity("Menu_Bar",
		RectSprite({ 60, 60, 60, 255 }),
		WindowScale(true, 1.0f, false, 0),
		HorizontalLayoutGroup(),
		RectTransform(0, 0, 500, 25, 10.0f, Alignment::TOP_LEFT)
	);
	menuBar.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::DontSave);
	HorizontalLayoutGroup* layoutGroup = menuBar.getComponent<HorizontalLayoutGroup>();
	layoutGroup->paddingLeft = 1;
	layoutGroup->paddingRight = 1;
	layoutGroup->spacing = 1;
	layoutGroup->childForceExpandWidth = false;
	layoutGroup->childForceExpandHeight = true;
	layoutGroup->childAlignment = Alignment::TOP_LEFT;
	// Drop menu
	EntityHandle menuFile = sceneUI->createEntity("Menu_Item_File",
		DropDown(Text("File", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255))),
		RectTransform(0, 0, 50, 20, 10.1f, Alignment::CENTER)
	);
	menuFile.setParent(menuBar);
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
	EntityHandle menuEdit = sceneUI->createEntity("Menu_Item_Edit",
		DropDown(Text("Edit", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255))),
		RectTransform(0, 0, 50, 20, 10.1f, Alignment::CENTER)
	);
	menuEdit.setParent(menuBar);
	file = menuEdit.getComponent<DropDown>();
	file->boxPaddingX = 5;
	file->boxPaddingY = 5;
	file->optionHeight = 25;
	file->boxWidth = 300;
	file->addOption("Undo", Core::bind(file, &DropDown::test));
	file->addOption("Redo", Core::bind(file, &DropDown::test));
	// Drop menu
	EntityHandle menuView = sceneUI->createEntity("Menu_Item_View",
		DropDown(Text("View", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255))),
		RectTransform(0, 0, 50, 20, 10.1f, Alignment::CENTER)
	);
	menuView.setParent(menuBar);
	file = menuView.getComponent<DropDown>();
	file->boxPaddingX = 5;
	file->boxPaddingY = 5;
	file->optionHeight = 25;
	file->boxWidth = 300;
	file->addOption("Test", Core::bind(file, &DropDown::test));
	// Drop menu
	EntityHandle menuObject = sceneUI->createEntity("Menu_Item_Object",
		DropDown(Text("Object", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255))),
		RectTransform(0, 0, 50, 20, 10.1f, Alignment::CENTER)
	);
	menuObject.setParent(menuBar);
	file = menuObject.getComponent<DropDown>();
	file->boxPaddingX = 5;
	file->boxPaddingY = 5;
	file->optionHeight = 25;
	file->boxWidth = 300;
	file->addOption("Create Empty Object", Core::bind(file, &DropDown::test));
	// ---------------------------------------------------Editor Panel-----------------------------------------------------------------
	// Right Panel
	EntityHandle rightPanel = sceneUI->createEntity("Right_Panel",
		EditorPanel(),
		RectSprite(Color(60,60,60,255)),
		WindowAnchor(Alignment::TOP_RIGHT, 0, 20),
		VerticalLayoutGroup(),
		DragAndResize(),
		RectTransform(0, 0, 400, 1000, 1.0f, Alignment::TOP_RIGHT)
	);
	DragAndResize* rpdar = rightPanel.getComponent<DragAndResize>();
	rpdar->top = false;
	rpdar->right = false;
	rpdar->left = true;
	rpdar->bottom = true;
	rpdar->edgeSize = 10;
	VerticalLayoutGroup* rplayoutGroup = rightPanel.getComponent<VerticalLayoutGroup>();
	rplayoutGroup->childForceExpandWidth = true;
	rplayoutGroup->childForceExpandHeight = true;
	rplayoutGroup->shrinkableChildWidth = true;
	rplayoutGroup->shrinkableChildHeight = true;
	rplayoutGroup->paddingTop = 5;
	rplayoutGroup->paddingLeft = 10;
	rplayoutGroup->paddingBottom = 10;
	rightPanel.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::DontSave);
	// -----------------------------------------------------GAME---------------------------------------------------------------------
	// Button
	EntityHandle button = sceneWorld->createEntity("Test_Button",
		Image("resources/images/invaders.png"),
		Text("Button!", "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255, 255)),
		Button("resources/images/invaders.png", "resources/images/gubbe.bmp", "resources/images/awesomeface.png"),
		Border(1, Color(255,255,255,255), editorSortingLayer),
		RectTransform(250, 250, 300, 300, 0.0f, Alignment::CENTER)
	);
	MouseDrag* mouseDrag = button.addComponent<MouseDrag>();
	mouseDrag->padding = 20;
	DragAndResize* behaviour = button.addComponent<DragAndResize>();
	behaviour->edgeSize = 20;
	behaviour->left = true;
	behaviour->right = true;
	behaviour->top = true;
	behaviour->bottom = true;

	// Object
	EntityHandle object = sceneWorld->createEntity("Test_Object",
		Image("resources/images/awesomeface.png"),
		MouseDrag(),
		RectTransform(100, 100, 350, 350, 0.0f, Alignment::CENTER)
	);

	// Child
	EntityHandle child = sceneWorld->createEntity("Test_Child",
		Image("resources/images/invaders.png"),
		MouseDrag(),
		RectTransform(200, 200, 200, 200, 0.0f, Alignment::CENTER)
	);
	child.setParent(object);

	// Child
	EntityHandle grandchild = sceneWorld->createEntity("Test_Grand_Child",
		Image("resources/images/invaders.png"),
		MouseDrag(),
		RectTransform(300, 200, 200, 200, 0.0f, Alignment::CENTER)
	);
	grandchild.setParent(child);

	// Level created, calling awake
	sceneUI->awake();
	sceneWorld->awake();

	// Keybinds
	engine.getInput().addAction("Terminate", GLFW_KEY_ESCAPE);
	engine.getInput().addAction("Save", GLFW_KEY_F5);
	engine.getInput().addAction("Load", GLFW_KEY_F9);

	engine.getInput().addAction("Camera_move_left", GLFW_KEY_LEFT);
	engine.getInput().addAction("Camera_move_up", GLFW_KEY_UP);
	engine.getInput().addAction("Camera_move_right", GLFW_KEY_RIGHT);
	engine.getInput().addAction("Camera_move_down", GLFW_KEY_DOWN);

	// Window hints
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	//Start game loop
	return engine.start();
}

void LevelEditor::keyPressed(std::string actionName) {
	if (actionName == "Save") {
		//engine.saveScene("level.dat");
	}
	else if (actionName == "Load") {
		//ScenePtr loadedLevel = engine.loadScene("level.dat");
	}
	else if (actionName == "Camera_move_left") {
		engine.getGraphics().getCamera().moveX(-100);
	}
	else if (actionName == "Camera_move_up") {
		engine.getGraphics().getCamera().moveY(-100);
	}
	else if (actionName == "Camera_move_right") {
		engine.getGraphics().getCamera().moveX(100);
	}
	else if (actionName == "Camera_move_down") {
		engine.getGraphics().getCamera().moveY(100);
	}
}
void LevelEditor::keyReleased(std::string buttonName) {
	if (buttonName == "Terminate")
		engine.terminate();
}