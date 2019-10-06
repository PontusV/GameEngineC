#include "Input.h"
#include "Engine.h"
#include "KeyListener.h"
#include "Algorithm.h"

#include "EntityManager.h"
#include "Component.h"
#include "Behaviour.h"
#include "Selectable.h"

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
	if (mouseMoved && leftMouseButtonPressed && lastClickTarget.refresh()) {
		std::vector<Behaviour*> scripts = lastClickTarget.getComponentsUpwards<Behaviour>();
		for (Behaviour* script : scripts) {
			script->onMouseDrag(mousePosition.x, mousePosition.y);
		}
		mouseMoved = false;
	}

	EntityHandle target = getEntityAtPos(mousePosition.x, mousePosition.y);
	// Mouse Hover
	if (target != hoverTarget) {
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

	// Clear old key pressed/released
	keysPressed.clear();
	keysReleased.clear();
	typedText = std::wstring();
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
	case INPUT_EVENT_CHARACTER:
		typeText(event.chr.codepoint);
		break;
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

				for (Selectable* script : selectedScripts) {
					script->deselect();
				}
				selectedScripts = target.getComponentsUpwards<Selectable>();
				for (Selectable* script : selectedScripts) {
					script->select();
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++) {
					scriptArray[i].onMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonPressedAsButton(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
				if (clickTime <= DOUBLE_CLICK_THRESHOLD && lastClickTarget.getEntity() == target.getEntity()) {
					for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
						script->onDoubleClick();
					}
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				leftMouseButtonPressed = false;
				if (target.getEntity() == lastClickTarget.getEntity()) {
					for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
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
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
		}
		else if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (event.mouseButton.action == GLFW_PRESS) {
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonReleased(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
			}
		}
		break;
	case INPUT_EVENT_SCROLL:
		for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
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

std::string Input::getKeyName(int keycode, int scancode) const {
	if (keycode == GLFW_KEY_SPACE) {
		return "space";
	}
	return glfwGetKeyName(keycode, scancode);
}

void Input::typeText(unsigned int codepoint) {
	wchar_t character = (wchar_t)codepoint;
	typedText += character;
}

const std::wstring& Input::getTextTyped() const {
	return typedText;
}

std::string Input::getButtonName(int keycode) const {
	std::map<int, std::string>::const_iterator pos = keyBinds.find(keycode);
	if (pos != keyBinds.end())
		return pos->second;
	else
		return "";
}

bool Input::getKeyDown(int keycode) const {
	return std::find(keysDown.begin(), keysDown.end(), keycode) != keysDown.end();
}

bool Input::getKeyPressed(int keycode) const {
	return std::find(keysPressed.begin(), keysPressed.end(), keycode) != keysPressed.end();
}

bool Input::getKeyReleased(int keycode) const {
	return std::find(keysReleased.begin(), keysReleased.end(), keycode) != keysReleased.end();
}

std::vector<int> Input::getKeysDown() {
	return keysDown;
}

std::vector<int> Input::getKeysPressed() {
	return keysPressed;
}

std::vector<int> Input::getKeysReleased() {
	return keysReleased;
}

void Input::keyPressed(const KeyEvent& event) {
	// Update list
	if (std::find(keysDown.begin(), keysDown.end(), event.keycode) == keysDown.end()) {
		keysDown.push_back(event.keycode);
		keysPressed.push_back(event.keycode);
	}
	//
	std::string buttonName = getButtonName(event.keycode);
	if (buttonName == "") //Return if no buttonName was found
		return;

	for (std::vector<KeyListener*>::const_iterator i = keyListeners.begin(); i != keyListeners.end(); ++i) { //Notify listeners
		(*i)->keyPressed(buttonName);
	}
}

void Input::keyReleased(const KeyEvent& event) {
	// Update list
	keysDown.erase(std::find(keysDown.begin(), keysDown.end(), event.keycode));
	keysReleased.push_back(event.keycode);
	//
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
	std::size_t spriteGroupSize = spriteGroup.size();

	for (std::size_t i = 0; i < spriteGroupSize; i++) {
		Sprite&			sprite		= spriteGroup.get<Sprite>(i);
		RectTransform&	transform	= spriteGroup.get<RectTransform>(i);

		// Add rectangles in view of window
		int cameraX = 0;
		int cameraY = 0;
		Camera& camera = engine->getGraphics().getCamera();
		Window& window = engine->getGraphics().getWindow();
		if (maths::isInsideWindow(cameraX, cameraY, window.getWidth(), window.getHeight(), transform)) {
			allRects.push_back({ transform, Texture2D(), i+1, sprite.getMasks() });
		}
	}

	// Filter out interactables that are not on mousePosition
	std::size_t index = maths::hitDetect(mousePosition.x, mousePosition.y, allRects);
	if (index == 0) return EntityHandle();
	index -= 1;
	if (index < spriteGroupSize) {
		Entity entity = spriteGroup.getEntity(index);
		Scene* scene = spriteGroup.get<ObjectData>(index).getScene();
		return EntityHandle(entity, scene);
	}

	return EntityHandle();
}

// -------------- ADD/SET/GET ------------------

void Input::setMousePosition(Vector2 position) {
	mouseMoved = true;
	mousePosition = position;
}

void Input::addInputEvent(const InputEvent& event) {
	events.push_back(event);
}

EntityHandle Input::getHoverTarget() {
	return hoverTarget;
}
const Vector2& Input::getMousePosition() const {
	return mousePosition;
}

EntityHandle Input::getLastClicked() {
	return lastClickTarget;
}