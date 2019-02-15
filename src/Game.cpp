#include "Game.h"
#include "Engine.h"
#include <iostream>
#include "level.h"
#include "EntityManager.h"
using namespace GameEngine;

Game::Game(Engine* engine) : engine(engine) {
	initiate();
}


Game::~Game()
{
}

#include "Transform.h"
#include "Image.h"
#include "Text.h"

//Test
EntityManager manager;
Entity object;

void Game::initiate() {
	//KeyListener
	engine->getInput().addKeyListener(this);
	engine->getInput().addKeyBind(SDLK_ESCAPE, "Terminate");
	engine->getInput().addKeyBind(SDLK_a, "Left");
	engine->getInput().addKeyBind(SDLK_d, "Right");

	//Create test level
	Level* level = engine->createLevel();

	//Test code
	object = manager.createEntity(
		new Transform(500, 500)
	);
	Entity object2 = manager.createEntity(
		new Transform(5, 5),
		new Image("D:/c++/wtf/invaders.png")
	);
	Entity object3 = manager.createEntity(
		new Transform(5, 500),
		new Image("D:/c++/wtf/invaders.png")
	);
	manager.addComponent(object, new Image("D:/c++/wtf/invaders.png"));
	manager.removeComponent<Image>(object2);
}

void Game::keyPressed(std::string buttonName) {
	if (buttonName == "Right") {
		Transform* transform = static_cast<Transform*>(manager.getComponent<Transform>(object));
		transform->setX(transform->getX() + 50);
	}
	if (buttonName == "Left") {
		Transform* transform = static_cast<Transform*>(manager.getComponent<Transform>(object));
		transform->setX(transform->getX() - 50);
	}
}

void Game::keyReleased(std::string buttonName) {
	if (buttonName == "Terminate")
		engine->terminate();
}
