#include "Input.h"
#include "SDL.h"
#include "Engine.h"
#include "KeyListener.h"

Input::Input(Engine* engine) : engine(engine) {}

Input::~Input() {
}

const std::string Input::getButtonName(int keyCode) const {
	std::map<int, std::string>::const_iterator pos = keyBinds.find(keyCode);
	if (pos != keyBinds.end())
		return pos->second;
	else
		return "";
}

void Input::keyPressed(int keyCode) const {
	std::string buttonName = getButtonName(keyCode);
	if (buttonName == "") //Return if no buttonName was found
		return;

	for (std::vector<KeyListener*>::const_iterator i = keyListeners.begin(); i != keyListeners.end(); ++i) { //Notify listeners
		(*i)->keyPressed(buttonName);
	}
	//Test code
	//keyBindsPressed[keyCode]
	//engine->terminate();
}

void Input::keyReleased(int keyCode) const {
	std::string buttonName = getButtonName(keyCode);
	if (buttonName == "") //Return if no buttonName was found
		return;

	for (std::vector<KeyListener*>::const_iterator i = keyListeners.begin(); i != keyListeners.end(); ++i) { //Notify listeners
		(*i)->keyReleased(buttonName);
	}
}
void Input::addKeyListener(KeyListener* listener) {
	keyListeners.push_back(listener);
}

void Input::addKeyBind(int keyCode, std::string buttonName) {
	keyBinds[keyCode] = buttonName;
}

/* Process input event */
void Input::processInputEvent(const SDL_Event& event) const {
	switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.repeat == 0)
				keyPressed(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			if (event.key.repeat == 0)
				keyReleased(event.key.keysym.sym);
			break;
		case SDL_QUIT:
			engine->terminate();
			break;
	}
}