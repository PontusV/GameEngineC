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
	if (mouseMoved) {
		mouseMoved = false;
		if (leftMouseButtonPressed && lastLeftClickTarget.refresh()) {
			std::vector<Behaviour*> scripts = lastLeftClickTarget.getComponentsUpwards<Behaviour>();
			for (Behaviour* script : scripts) {
				script->onMouseDrag(mousePosition.x, mousePosition.y);
			}
		}
	}

	EntityHandle target = getEntityAtPos(mousePosition.x, mousePosition.y);
	// Mouse Hover
	if (target != hoverTarget) {
		// Hover out
		if (!target.isParent(hoverTarget.getEntity())) {
			for (Behaviour* script : hoverTarget.getComponents<Behaviour>()) {
				script->onHoverout();
			}
			// Hover out - parents
			Handle parent = hoverTarget.getParent();
			while (parent.refresh()) {
				if (parent == target) break;
				for (Behaviour* script : parent.getComponents<Behaviour>()) {
					script->onHoverout();
				}
				parent = parent.getParent();
			}
		}
		// Hover over
		if (!hoverTarget.isParent(target.getEntity())) {
			for (Behaviour* script : target.getComponents<Behaviour>()) {
				script->onHoverover();
			}
			// Hover over - parents
			Handle parent = target.getParent();
			while (parent.refresh()) {
				if (parent == hoverTarget) break;
				for (Behaviour* script : parent.getComponents<Behaviour>()) {
					script->onHoverover();
				}
				parent = parent.getParent();
			}
		}

		// Switch current hover ptr
		hoverTarget = target;
	}

	// Clear old key pressed/released
	keysPressed.clear();
	keysReleased.clear();
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
		if (hasKeyboardFocus()) {
			if (event.key.action == GLFW_RELEASE && event.key.keycode == GLFW_KEY_ESCAPE) {
				// Deselect. TODO: Put deselect in function
				for (Selectable* script : selectedTarget.getComponentsUpwards<Selectable>()) {
					script->deselect();
				}
				selectedTarget = EntityHandle();
				return;
			}
		}
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

				lastLeftClickTarget = target;
				leftMouseButtonPressed = true;
				timeSinceLastClick = (float)glfwGetTime();

				for (Selectable* script : selectedTarget.getComponentsUpwards<Selectable>()) {
					script->deselect();
				}
				selectedTarget = target;
				for (Selectable* script : selectedTarget.getComponentsUpwards<Selectable>()) {
					script->select();
				}
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonPressedAsButton(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
				if (clickTime <= DOUBLE_CLICK_THRESHOLD && lastLeftClickTarget.getEntity() == target.getEntity()) {
					for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
						script->onDoubleClick();
					}
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++) {
					scriptArray[i].onMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				leftMouseButtonPressed = false;
				if (target.getEntity() == lastLeftClickTarget.getEntity()) {
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
				lastRightClickTarget = target;
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonPressedAsButton(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++) {
					scriptArray[i].onMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				if (target.getEntity() == lastRightClickTarget.getEntity()) {
					for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
						script->onMouseButtonReleasedAsButton(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
					}
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++) {
					scriptArray[i].onMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT, event.mouseButton.mods);
				}
			}
		}
		else if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (event.mouseButton.action == GLFW_PRESS) {
				lastMiddleClickTarget = target;
				for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
					script->onMouseButtonPressedAsButton(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++) {
					scriptArray[i].onMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				if (target.getEntity() == lastMiddleClickTarget.getEntity()) {
					for (Behaviour* script : target.getComponentsUpwards<Behaviour>()) {
						script->onMouseButtonReleasedAsButton(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
					}
				}
				ComponentArray<Behaviour>& scriptArray = engine->getBehaviourManager().getAllScripts();
				for (std::size_t i = 0; i < scriptArray.size(); i++) {
					scriptArray[i].onMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE, event.mouseButton.mods);
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
	if (!hasKeyboardFocus()) return;
	wchar_t character = (wchar_t)codepoint;
	std::wstring text;
	text.push_back(character);
	keyboardFocus.get()->write(text);
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

void Input::keyPressed(const KeyEvent& event) {
	// Update list
	if (std::find(keysDown.begin(), keysDown.end(), event.keycode) == keysDown.end()) {
		keysDown.push_back(event.keycode);
		keysPressed.push_back(event.keycode);
	}
	if (hasKeyboardFocus()) return;

	// Find Action
	auto it = keyBinds.find(ButtonInput(event.keycode, event.mods));
	if (it == keyBinds.end()) return;
	const Action& action = it->second;

	// Notify keylisteners
	for (std::vector<KeyListener*>::const_iterator i = keyListeners.begin(); i != keyListeners.end(); ++i) { //Notify listeners
		(*i)->keyPressed(action.name);
	}

	// Call Action callback
	auto it2 = action.callbacks.find(Action::PRESS);
	std::vector<ComponentFunctionHandle<void>> callbacks = it2 == action.callbacks.end() ? std::vector<ComponentFunctionHandle<void>>() : it2->second;
	for (auto iterator = callbacks.begin(); iterator != callbacks.end();) {
		ComponentFunctionHandle<void>& callback = *iterator;
		if (callback.isValid()) {
			callback.invoke();
			it++;
		}
		else {
			// Removes invalid callback
			callbacks.erase(iterator);
		}
	}
}

void Input::keyReleased(const KeyEvent& event) {
	// Update list
	keysDown.erase(std::find(keysDown.begin(), keysDown.end(), event.keycode));
	keysReleased.push_back(event.keycode);
	if (hasKeyboardFocus()) return;

	// Find Action
	auto it = keyBinds.find(ButtonInput(event.keycode, event.mods));
	if (it == keyBinds.end()) return;
	const Action& action = it->second;

	// Notify keylisteners
	for (std::vector<KeyListener*>::const_iterator i = keyListeners.begin(); i != keyListeners.end(); ++i) { //Notify listeners
		(*i)->keyReleased(action.name);
	}

	// Call Action callback
	auto it2 = action.callbacks.find(Action::RELEASE);
	std::vector<ComponentFunctionHandle<void>> callbacks = it2 == action.callbacks.end() ? std::vector<ComponentFunctionHandle<void>>() : it2->second;
	for (auto iterator = callbacks.begin(); iterator != callbacks.end();) {
		ComponentFunctionHandle<void>& callback = *iterator;
		if (callback.isValid()) {
			callback.invoke();
			it++;
		}
		else {
			// Removes invalid callback
			callbacks.erase(iterator);
		}
	}
}

void Input::addKeyListener(KeyListener* listener) {
	keyListeners.push_back(listener);
}

void Input::addAction(std::string actionName, int buttoncode, int modFlags) {
	keyBinds[ButtonInput(buttoncode, modFlags)] = Action(actionName, modFlags);
}

void Input::addActionCallback(std::string actionName, int actioncode, ComponentFunctionHandle<void> function) {
	for (auto it = keyBinds.begin(); it != keyBinds.end(); it++) {
		Action& action = it->second;
		if (action.name == actionName) {
			action.callbacks[actioncode].push_back(function);
			return;
		}
	}
	std::cout << "An action with the given name (" << actionName << ") does not exist!" << std::endl;
	throw std::invalid_argument("An action with the given name does not exist!");
}

void Input::focusKeyboard(TComponentHandle<InputField> entity) {
	keyboardFocus = entity;
}

void Input::clearFocusKeyboard() {
	keyboardFocus.clear();
}

TComponentHandle<InputField> Input::getKeyboardFocus() {
	return keyboardFocus;
}

bool Input::hasKeyboardFocus() {
	return keyboardFocus.isValid();
}

struct HitDetectData {
	HitDetectData() : index(-1), sortingOrder(0) {}
	std::size_t index;
	float sortingOrder;
};

// -------------- HELPERS ------------------
EntityHandle Input::getEntityAtPos(float x, float y) {
	std::size_t spriteGroupSize = spriteGroup.size();
	std::size_t spriteGroupUISize = spriteGroupUI.size();
	HitDetectData currentHit;
	
	// --------------------------------- UI -------------------------------------
	for (std::size_t i = 0; i < spriteGroupUISize; i++) {
		Sprite&			sprite		= spriteGroupUI.get<Sprite>(i);
		RectTransform&	transform	= spriteGroupUI.get<RectTransform>(i);

		if (transform.getZ() < currentHit.sortingOrder) continue;

		// Add rectangles in view of window
		Window& window = engine->getGraphics().getWindow();
		std::array<Vector2, 4> vertices = transform.getVertices();

		if (maths::isInsideWindow(window.getWidth(), window.getHeight(), vertices)) {
			// Simple hit detection
			if (maths::hitCheck(mousePosition.x, mousePosition.y, vertices) && maths::hitCheckCollection(mousePosition.x, mousePosition.y, sprite.getMasks())) {
				currentHit.index = i;
				currentHit.sortingOrder = transform.getZ();
			}
		}
	}
	if (currentHit.index < spriteGroupUISize) {
		Entity entity = spriteGroupUI.getEntity(currentHit.index);
		Scene* scene = spriteGroupUI.get<UIObjectData>(currentHit.index).getScene();
		return EntityHandle(entity, scene);
	}

	// --------------------------------- World -------------------------------------
	for (std::size_t i = 0; i < spriteGroupSize; i++) {
		Sprite&			sprite		= spriteGroup.get<Sprite>(i);
		RectTransform	transform	= spriteGroup.get<RectTransform>(i);

		if (transform.getZ() < currentHit.sortingOrder) continue;

		// Add rectangles in view of window
		Window& window = engine->getGraphics().getWindow();
		Camera& camera = engine->getGraphics().getCamera();
		std::array<Vector2, 4> vertices = transform.getVertices(camera.getViewMatrix());

		if (maths::isInsideWindow(window.getWidth(), window.getHeight(), vertices)) {
			// Simple hit detection
			if (maths::hitCheck(mousePosition.x, mousePosition.y, vertices) && maths::hitCheckCollection(mousePosition.x, mousePosition.y, sprite.getMasks())) {
				currentHit.index = i;
				currentHit.sortingOrder = transform.getZ();
			}
		}
	}

	if (currentHit.index < spriteGroupSize) {
		Entity entity = spriteGroup.getEntity(currentHit.index);
		Scene* scene = spriteGroup.get<GameObjectData>(currentHit.index).getScene();
		return EntityHandle(entity, scene);
	}

	// Miss
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

EntityHandle Input::getLastLeftClicked() {
	return lastLeftClickTarget;
}