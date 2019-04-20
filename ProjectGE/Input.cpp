#include "Input.h"
#include "Engine.h"
#include "KeyListener.h"
#include "Algorithm.h"

#include "EntityManager.h"
#include "Level.h"
#include "Component.h"
#include "Script.h"

#include <GLFW/glfw3.h>
#include <memory>
using namespace Core;
using namespace Core::ui;

Input::Input(Engine* engine) : engine(engine) {
	mouseMoved = true;
	leftMouseButtonPressed = false;
	timeSinceLastClick = 0.0f;
}

Input::~Input() {
}

void Input::update(float dt) {
	// Mouse Drag
	if (mouseMoved && leftMouseButtonPressed) {
		std::vector<Script*> scripts = lastClickTarget.getComponentsUpwards<Script>();
		for (Script* script : scripts) {
			script->onMouseDrag(mousePosition.x, mousePosition.y);
		}
	}

	EntityHandle target = getEntityAtPos(mousePosition.x, mousePosition.y);
	// Mouse Hover
	if (target.getEntity() != hoverTarget.getEntity()) {
		std::vector<Script*> scripts = hoverTarget.getComponentsUpwards<Script>();
		for (Script* script : scripts) {
			script->onHoverout();
		}
		// Switch current hover ptr
		hoverTarget = target;

		scripts = hoverTarget.getComponentsUpwards<Script>();
		for (Script* script : scripts) {
			script->onHoverover();
		}
	}
	
	// Process input events
	for (InputEvent& event : events) {
		processInputEvent(event, target);
	}

	// Clear input events
	events.clear();
}

/* Process input event */
void Input::processInputEvent(const InputEvent& event, EntityHandle& target) {
	switch (event.type) {
	case INPUT_EVENT_KEY:
		if (event.key.action == GLFW_PRESS) {
			keyPressed(event.key);
		}
		else if (event.key.action == GLFW_RELEASE) {
			keyReleased(event.key);
		}
		break;
	case INPUT_EVENT_MOUSEBUTTON:
		if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_LEFT) {
			if (event.mouseButton.action == GLFW_PRESS) {
				float clickTime = (float)glfwGetTime() - timeSinceLastClick;

				lastClickTarget = target;
				leftMouseButtonPressed = true;
				timeSinceLastClick = (float)glfwGetTime();

				std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
				for (Script* script : scripts) {
					script->onMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
				if (clickTime <= DOUBLE_CLICK_THRESHOLD && lastClickTarget.getEntity() == target.getEntity()) {
					std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
					for (Script* script : scripts) {
						script->onDoubleClick();
					}
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				leftMouseButtonPressed = false;
				std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
				for (Script* script : scripts) {
					script->onMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
			}
		}
		else if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_RIGHT) {
			if (event.mouseButton.action == GLFW_PRESS) {
				std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
				for (Script* script : scripts) {
					script->onMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
				for (Script* script : scripts) {
					script->onMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
		}
		else if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (event.mouseButton.action == GLFW_PRESS) {
				std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
				for (Script* script : scripts) {
					script->onMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
				for (Script* script : scripts) {
					script->onMouseButtonReleased(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
			}
		}
		break;
	case INPUT_EVENT_SCROLL:
		std::vector<Script*> scripts = target.getComponentsUpwards<Script>();
		for (Script* script : scripts) {
			script->onScroll(event.scroll.xoffset, event.scroll.yoffset);
		}
		break;
	}
}

// ------------------------------- MOUSE -----------------------------------
void Input::mouseButtonPressed(const MouseButtonEvent& event) const {

}

void Input::mouseButtonReleased(const MouseButtonEvent& event) const {

}

// ------------------------------- KEY -----------------------------------
const std::string Input::getButtonName(int keycode) const {
	std::map<int, std::string>::const_iterator pos = keyBinds.find(keycode);
	if (pos != keyBinds.end())
		return pos->second;
	else
		return "";
}

void Input::keyPressed(const KeyEvent& event) const {
	std::string buttonName = getButtonName(event.keycode);
	if (buttonName == "") //Return if no buttonName was found
		return;

	for (std::vector<KeyListener*>::const_iterator i = keyListeners.begin(); i != keyListeners.end(); ++i) { //Notify listeners
		(*i)->keyPressed(buttonName);
	}
}

void Input::keyReleased(const KeyEvent& event) const {
	std::string buttonName = getButtonName(event.keycode);
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

// -------------- HELPERS ------------------
EntityHandle Input::getEntityAtPos(float x, float y) {
	std::vector<maths::TexturedRectTransform> allRects;
	for (std::size_t i = 0; i < texturedEntities.transforms.size(); i++) {
		Image&		image		= texturedEntities.images[i];
		Transform&	transform	= texturedEntities.transforms[i];

		// Add rectangles in view of window
		maths::RectTransform rect{transform, image.getTexture().size};
		int cameraX = 0;
		int cameraY = 0;
		if (maths::isInsideWindow(cameraX, cameraY, engine->getGraphics().getWindow().getWidth(), engine->getGraphics().getWindow().getHeight(), rect)) {
			allRects.push_back({ image.getTexture(), { transform, image.getTexture().size, (glm::vec2)image.getSize() * transform.getAnchor() }, (int)i });
		}
	}

	// Filter out interactables that are not on mousePosition
	int index = maths::hitDetect(mousePosition.x, mousePosition.y, allRects);
	if (index == -1) return EntityHandle();
	LevelPtr						level	= engine->getCurrentLevel();
	Entity							entity	= texturedEntities.images.getEntity(index);

	return level->getEntityHandle(entity);
}

// -------------- ADD/SET/GET ------------------

void Input::setMousePos(glm::vec2 position) {
	mouseMoved = true;
	mousePosition = position;
}

void Input::addInputEvent(const InputEvent& event) {
	events.push_back(event);
}