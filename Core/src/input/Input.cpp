#include "Input.h"
#include "engine/Engine.h"
#include "KeyListener.h"
#include "Maths/Algorithm.h"

#include "entity/EntityManager.h"
#include "entity/component/Component.h"
#include "components/Behaviour.h"
#include "components/ui/input/Selectable.h"

#include <GLFW/glfw3.h>
#include <memory>
using namespace Core;

const float Input::SELECT_NEXT_DELAY = 0.05f; // The delay before selecting next
const float Input::INITIAL_SELECT_NEXT_DELAY = 0.5f; // The initial delay before repeating selectNext

Input::Input(Engine* engine) : engine(engine) {
	mouseMoved = true;
	leftMouseButtonPressed = false;
	timeSinceLastClick = 0.0f;
}

Input::~Input() {
}

void Input::update(float deltaTime) {
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
				script->onHoverOut();
			}
			// Hover out - parents
			Handle parent = hoverTarget.getParent();
			while (parent.refresh()) {
				if (parent == target) break;
				for (Behaviour* script : parent.getComponents<Behaviour>()) {
					script->onHoverOut();
				}
				parent = parent.getParent();
			}
		}
		// Hover over
		if (!hoverTarget.isParent(target.getEntity())) {
			for (Behaviour* script : target.getComponents<Behaviour>()) {
				script->onHoverOver();
			}
			// Hover over - parents
			Handle parent = target.getParent();
			while (parent.refresh()) {
				if (parent == hoverTarget) break;
				for (Behaviour* script : parent.getComponents<Behaviour>()) {
					script->onHoverOver();
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
	if (selectNextHeldDown) {
		if (timeBeforeNextSelect <= 0.0) {
			timeBeforeNextSelect += SELECT_NEXT_DELAY;
			selectNext();
		}
		timeBeforeNextSelect -= deltaTime;
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
		if (hasKeyboardFocus()) {
			if (event.key.action == GLFW_RELEASE && event.key.keycode == GLFW_KEY_ESCAPE) {
				deselect();
				selectedTarget = EntityHandle();
				return;
			}
			if (event.key.keycode == GLFW_KEY_TAB) {
				if (event.key.action == GLFW_PRESS) {
					selectNextHeldDown = true;
					timeBeforeNextSelect = INITIAL_SELECT_NEXT_DELAY;
					selectNext();
				}
				else if (event.key.action == GLFW_RELEASE)
					selectNextHeldDown = false;
				return;
			}
		}
		onKeyEvent(event.key);
		break;
	case INPUT_EVENT_MOUSEBUTTON:
		if (event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_LEFT) {
			if (event.mouseButton.action == GLFW_PRESS) {
				float clickTime = (float)glfwGetTime() - timeSinceLastClick;

				lastLeftClickTarget = target;
				leftMouseButtonPressed = true;
				timeSinceLastClick = (float)glfwGetTime();

				select(target);
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
// ------------------------------- SELECT -----------------------------------

void Input::deselect() {
	for (Selectable* script : selectedTarget.getComponentsUpwards<Selectable>()) {
		script->deselect();
	}
}

void Input::select(EntityHandle target) {
	deselect();
	selectedTarget = target;
	for (Selectable* script : selectedTarget.getComponentsUpwards<Selectable>()) {
		script->select();
	}
}

void Input::selectNext() {
	// Find next selectable
	for (Selectable* script : selectedTarget.getComponentsUpwards<Selectable>()) {
		EntityHandle nextSelect = script->getNext();
		if (nextSelect.refresh()) {
			// Goes to next selectable
			select(nextSelect);
			return;
		}
	}
}

// ------------------------------- MOUSE -----------------------------------

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

void Input::onKeyEvent(const KeyEvent& event) {
	if (hasKeyboardFocus()) return;
	// Find Action
	auto it = keyBinds.find(ButtonInput(event.keycode, event.mods));
	if (it == keyBinds.end()) return;
	const Action& action = it->second;

	// Notify keylisteners
	for (std::vector<KeyListener*>::const_iterator i = keyListeners.begin(); i != keyListeners.end(); ++i) { //Notify listeners
		if (event.action == Action::PRESS)
			(*i)->keyPressed(action.name);
		else if (event.action == Action::RELEASE)
			(*i)->keyReleased(action.name);
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
			iterator = callbacks.erase(iterator);
		}
	}
}

void Input::keyPressed(const KeyEvent& event) {
	// Update list
	if (std::find(keysDown.begin(), keysDown.end(), event.keycode) == keysDown.end()) {
		keysDown.push_back(event.keycode);
		keysPressed.push_back(event.keycode);
	}
}

void Input::keyReleased(const KeyEvent& event) {
	// Update list
	keysDown.erase(std::find(keysDown.begin(), keysDown.end(), event.keycode));
	keysReleased.push_back(event.keycode);
}

void Input::addKeyListener(KeyListener* listener) {
	keyListeners.push_back(listener);
}

void Input::addAction(std::string actionName, int buttoncode, int modFlags) {
	keyBinds.insert(std::make_pair(ButtonInput(buttoncode, modFlags), Action(actionName, modFlags)));
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

/* Utility */
bool contains(const std::vector<Entity>& list, const Entity& value) {
	for (const Entity& entry : list) {
		if (entry == value) return true;
	}
	return false;
}

// -------------- HELPERS ------------------
EntityHandle Input::getEntityAtPos(float x, float y, std::vector<Entity> ignoreList) {
	std::size_t spriteGroupSize = spriteGroup.size();
	HitDetectData currentHit;

	// --------------------------------- World -------------------------------------
	Camera& camera = engine->getGraphics().getCamera();
	Window& window = engine->getGraphics().getWindow();
	for (std::size_t i = 0; i < spriteGroupSize; i++) {
		Entity&			entity		= spriteGroup.getEntity(i);
		Sprite&			sprite		= spriteGroup.get<Sprite>(i);
		RectTransform	transform	= spriteGroup.get<RectTransform>(i);

		if (contains(ignoreList, entity)) continue;
		if (transform.getZ() < currentHit.sortingOrder) continue;

		// Add rectangles in view of window
		std::array<Vector2, 4> vertices = transform.isInWorldSpace() ? transform.getVertices(camera.getViewMatrix()) : transform.getVertices();

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
		Scene* scene = spriteGroup.get<RectTransform>(currentHit.index).getOwner().getScene(); // TODO: Find a prettier way to do this?
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