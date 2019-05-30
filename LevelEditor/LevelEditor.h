#ifndef EDITOR_H
#define EDITOR_H
#include "../Core/Core.h"
#include "../Core/Engine.h"
#include "../Core/KeyListener.h"
// Core.h
#include "../Core/Level.h"
#include "../Core/RectTransform.h"
#include "../Core/DropDown.h"
#include "../Core/Button.h"
#include "../Core/WindowAnchor.h"
#include "../Core/MouseDrag.h"
#include "../Core/LayoutElement.h"
#include "../Core/HorizontalLayoutGroup.h"
#include "../Core/WindowScale.h"
#include "../Core/DragAndResize.h"
#include "../Core/Inspector.h"

class LevelEditor :
	public Core::KeyListener
{
public:
	LevelEditor();
	~LevelEditor();
	int initiate();
	void keyPressed(std::string buttonName);
	void keyReleased(std::string buttonName);
private:
	Core::Engine engine;
};
#endif