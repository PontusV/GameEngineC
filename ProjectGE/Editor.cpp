#include "Editor.h"
using namespace LevelEditor;
using namespace Core;

Editor::Editor()
{
	initiate();
	//Start game engine
	engine.initiate();
}


Editor::~Editor()
{
}


void Editor::initiate() {
	//engine.getGraphics().createWindow("Level Editor", 0, 0, 1680, 1050);
	//KeyListener
	engine.getInput().addKeyListener(this);
	/*engine.getInput().addKeyBind(SDLK_ESCAPE, "Terminate");
	engine.getInput().addKeyBind(SDLK_F5, "Save");
	engine.getInput().addKeyBind(SDLK_F9, "Load");*/

	//Create test level
	LevelPtr level = engine.createLevel();
}
void Editor::keyPressed(std::string buttonName) {
	if (buttonName == "Save") {
		engine.saveLevel();
	}
	if (buttonName == "Load") {
		engine.loadLevel();
	}
}
void Editor::keyReleased(std::string buttonName) {
	if (buttonName == "Terminate")
		engine.terminate();
}