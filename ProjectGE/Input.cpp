#include "Input.h"
#include "Engine.h"
#include "KeyListener.h"
#include "Algorithm.h"

#include "EntityManager.h"
#include "Level.h"
#include "Component.h"
#include "Behaviour.h"

#include <GLFW/glfw3.h>
#include <memory>
using namespace Core;

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
		std::vector<Behaviour*> scripts = lastClickTarget.getComponentsUpwards<Behaviour>();
		for (Behaviour* script : scripts) {
			script->onMouseDrag(mousePosition.x, mousePosition.y);
		}
	}

	EntityHandle target = getEntityAtPos(mousePosition.x, mousePosition.y);
	// Mouse Hover
	if (target.getEntity() != hoverTarget.getEntity()) {
		std::vector<Behaviour*> scripts;
		// Hover out
		if (!hoverTarget.isChild(target.getEntity())) { // Is the new hoverTarget a child of the previous one? If true, no onHoverOut is called
			scripts = hoverTarget.getComponents<Behaviour>();
			for (Behaviour* script : scripts) {
				script->onHoverout();
			}
			// Hover out - parents
			Handle parent = hoverTarget.getParent();
			while (parent.refresh()) {
				hoverTarget = parent;
				if (hoverTarget.getEntity() == target.getEntity()) break;
				scripts = hoverTarget.getComponents<Behaviour>();
				for (Behaviour* script : scripts) {
					script->onHoverout();
				}
				parent = hoverTarget.getParent();
			}
		}

		// Switch current hover ptr
		EntityHandle prevTarget = hoverTarget;
		hoverTarget = target;

		// Hover over
		if (!target.isChild(prevTarget.getEntity())) { // Is the previous hoverTarget a child of the new one? If true, no onHoverOver is called
			scripts = target.getComponents<Behaviour>();
			for (Behaviour* script : scripts) {
				script->onHoverover();
			}
			// Hover over - parents
			Handle parent = target.getParent();
			while (parent.refresh()) {
				target = parent;
				if (target.getEntity() == prevTarget.getEntity()) break;
				scripts = target.getComponents<Behaviour>();
				for (Behaviour* script : scripts) {
					script->onHoverover();
				}
				parent = target.getParent();
			}
		}
	}
	
	// Process input events
	for (InputEvent& event : events) {
		processInputEvent(event, hoverTarget);
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

				std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
				for (Behaviour* script : scripts) {
					script->onMouseButtonPressedAsButton(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++) {
					scriptArray[i].onMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
				if (clickTime <= DOUBLE_CLICK_THRESHOLD && lastClickTarget.getEntity() == target.getEntity()) {
					std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
					for (Behaviour* script : scripts) {
						script->onDoubleClick();
					}
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				leftMouseButtonPressed = false;
				std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
				if (target.getEntity() == lastClickTarget.getEntity()) {
					for (Behaviour* script : scripts) {
						script->onMouseButtonReleasedAsButton(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
					}
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++){
					scriptArray[i].onMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
			}
		}
		else if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_RIGHT) {
			if (event.mouseButton.action == GLFW_PRESS) {
				std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
				for (Behaviour* script : scripts) {
					script->onMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
				for (Behaviour* script : scripts) {
					script->onMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
		}
		else if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (event.mouseButton.action == GLFW_PRESS) {
				std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
				for (Behaviour* script : scripts) {
					script->onMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
				for (Behaviour* script : scripts) {
					script->onMouseButtonReleased(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
			}
		}
		break;
	case INPUT_EVENT_SCROLL:
		std::vector<Behaviour*> scripts = target.getComponentsUpwards<Behaviour>();
		for (Behaviour* script : scripts) {
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
std::string Input::getButtonName(int keycode) const {
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
	std::vector<maths::RectTransformEntry> allRects;

	for (std::size_t i = 0; i < spriteGroup.transforms.size(); i++) {
		Sprite&			sprite		= spriteGroup.sprites[i];
		RectTransform&	transform	= spriteGroup.transforms[i];

		// Add rectangles in view of window
		int cameraX = 0;
		int cameraY = 0;
		Window& window = engine->getGraphics().getWindow();
		if (maths::isInsideWindow(cameraX, cameraY, window.getWidth(), window.getHeight(), transform)) {
			allRects.push_back({ transform, Texture2D(), i+1 });
		}
	}

	// Filter out interactables that are not on mousePosition
	std::size_t index = maths::hitDetect(mousePosition.x, mousePosition.y, allRects);
	if (index == 0) return EntityHandle();
	index -= 1;
	LevelPtr level = engine->getCurrentLevel();
	Entity entity;
	if (index < spriteGroup.transforms.size()) {
		entity = spriteGroup.sprites.getEntity(index);
	}

	return level->getEntityHandle(entity);
}

// -------------- ADD/SET/GET ------------------

void Input::setMousePosition(glm::vec2 position) {
	mouseMoved = true;
	mousePosition = position;
}

void Input::addInputEvent(const InputEvent& event) {
	events.push_back(event);
}

EntityHandle Input::getHoverTarget() {
	return hoverTarget;
}
const glm::vec2& Input::getMousePosition() const {
	return mousePosition;
}

EntityHandle Input::getLastClicked() {
	return lastClickTarget;
}