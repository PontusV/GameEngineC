#ifndef EDITOR_H
#define EDITOR_H
#include "engine/Engine.h"
#include "input/KeyListener.h"
#include <string>

class LevelEditor : public Core::KeyListener {
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