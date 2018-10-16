#include "Game.h"
#include "Engine.h"
#include <iostream>
#include "level.h"
#include "EntityHandle.h"

//TEST
#include "ComponentListener.h"

Game::Game(Engine* engine) : engine(engine) {
	initiate();
}


Game::~Game()
{
}

void Game::initiate() {
	//KeyListener
	engine->getInput().addKeyListener(this);
	engine->getInput().addKeyBind(SDLK_ESCAPE, "Terminate");
	engine->getInput().addKeyBind(SDLK_a, "Left");
	engine->getInput().addKeyBind(SDLK_d, "Right");

	//Create test level
	Level* level = engine->createLevel();

	for (int i = 0; i < 1000; i++) {
		std::string name = std::string("Test") + std::to_string(i);
		EntityHandle entity = engine->getCurrentLevel()->createEntity(name);
		entity.addComponent<Transform>((float)500, (float)100);
		entity.addComponent<Image>("G:/c++/wtf/invaders.png");
	}
}

void Game::keyPressed(std::string buttonName) {
	if (buttonName == "Right") {
		for (int i = 0; i < 1000; i++) {
			std::string name = std::string("Test") + std::to_string(i);
			EntityHandle entity = engine->getCurrentLevel()->getEntity(name);

			Transform component = entity.getComponent<Transform>();
			component.setX(component.getX() + 10);
			entity.updateComponent<Transform>(component);
		}

	}
	if (buttonName == "Left") {
		EntityHandle entity = engine->getCurrentLevel()->getEntity("Test3");
		Transform component = entity.getComponent<Transform>();
		component.setX(component.getX() - 10);
		entity.updateComponent<Transform>(component);
	}
}

void Game::keyReleased(std::string buttonName) {
	if (buttonName == "Terminate")
		engine->terminate();
}
