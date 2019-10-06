#include "LevelEditor.h"
#include <GLFW/glfw3.h>

#include "../Core/Scene.h"
#include "../Core/RectTransform.h"
#include "../Core/DropDown.h"
#include "../Core/Button.h"
#include "../Core/WindowAnchor.h"
#include "../Core/MouseDrag.h"
#include "../Core/LayoutElement.h"
#include "../Core/HorizontalLayoutGroup.h"
#include "../Core/VerticalLayoutGroup.h"
#include "../Core/WindowScale.h"
#include "../Core/DragAndResize.h"
#include "../Core/Inspector.h"

#include "../Core/Maths/Vector2.h"
#include "../Core/Maths/Vector3.h"

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
	window.setBackgroundColor(Vector3(50, 50, 50));

	// Create Level Editor
	ScenePtr sceneUI = engine.getSceneManager().createScene("Level_Editor", ObjectType::UI);
	ScenePtr sceneWorld = engine.getSceneManager().createScene("World", ObjectType::World);
	unsigned char editorSortingLayer = 0;
	// ------------------------------------------------------Menu bar----------------------------------------------------------
	EntityHandle menuBar = sceneUI->createEntity("Menu_Bar",
		RectSprite({ 60, 60, 60, 255 }),
		WindowScale(true, 1.0f, false, 0),
		RectTransform(0, 0, 500, 25, 10.0f, Alignment::TOP_LEFT)
	);
	menuBar.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::DontSave);
	HorizontalLayoutGroup* layoutGroup = menuBar.addComponent<HorizontalLayoutGroup>();
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
	// ---------------------------------------------------Inspector-----------------------------------------------------------------
	// Right Panel.
	int inspectorWidth = 400;
	int inspectorHeight = 1000;
	EntityHandle rightPanel = sceneUI->createEntity("Right_Panel",
		RectSprite(Color(60,60,60,255)),
		WindowAnchor(Alignment::TOP_RIGHT, 0, 20),
		VerticalLayoutGroup(),
		DragAndResize(),
		RectTransform(1000, 50, inspectorWidth, inspectorHeight, 1.0f, Alignment::TOP_RIGHT)
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
	int textPadding = 5;
	int backgroundPadding = 3;
	Text inspectorLabelText = Text("Inspector", "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255, 255));
	Vector2 inspectorLabelSize = inspectorLabelText.getSize();
	int labelRectWidth = inspectorLabelSize.x + textPadding * 2;
	int labelRectHeight = inspectorLabelSize.y + textPadding * 2;
	// Inspector label background
	EntityHandle inspectorLabelRect = sceneUI->createEntity("Inspector_label_background",
		RectSprite(Color(100, 100, 100, 255)),
		LayoutElement(),
		RectTransform(0, 0, labelRectWidth, labelRectHeight, 1.05f, Alignment::TOP_LEFT)
	);
	LayoutElement* labelRectLE = inspectorLabelRect.getComponent<LayoutElement>();
	labelRectLE->setMinSize(Vector2(labelRectWidth, labelRectHeight));
	labelRectLE->setMinSizeEnabled(true);
	labelRectLE->setFlexibleSize(Vector2(0.0f, 0.0f));
	labelRectLE->setFlexibleSizeEnabled(true);
	inspectorLabelRect.setParent(rightPanel);
	// Inspector label
	EntityHandle inspectorLabel = sceneUI->createEntity("Inspector_label",
		inspectorLabelText,
		RectTransform(textPadding, textPadding, labelRectWidth, labelRectHeight, 1.1f, Alignment::TOP_LEFT)
	);
	inspectorLabel.setParent(inspectorLabelRect);

	// Inspector background
	EntityHandle inspector = sceneUI->createEntity("Inspector_background",
		Inspector(),
		RectSprite(Color(175, 0, 0, 255)),
		RectTransform(0, 0, 0, 0, 1.05f, Alignment::TOP_LEFT)
	);
	inspector.setParent(rightPanel);
	//*/
	//------------------------------------------------------GAME---------------------------------------------------------------------
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

	// Level created, calling awake
	sceneUI->awake();
	sceneWorld->awake();

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
		//engine.saveScene("level.dat");
	}
	if (buttonName == "Load") {
		//ScenePtr loadedLevel = engine.loadScene("level.dat");
	}
}
void LevelEditor::keyReleased(std::string buttonName) {
	if (buttonName == "Terminate")
		engine.terminate();
}