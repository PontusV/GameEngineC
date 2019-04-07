#ifndef EDITOR_H
#define EDITOR_H
#include "../ProjectGE/Engine.h"
#include "../ProjectGE/KeyListener.h"

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