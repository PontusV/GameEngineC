#include "Input.h"
#include "Engine.h"
#include "KeyListener.h"
#include "Algorithm.h"

#include "EntityManager.h"
#include "Level.h"
#include "Component.h"

#include <GLFW/glfw3.h>
#include <memory>
using namespace Core;
using namespace Core::ui;

Input::Input(Engine* engine) : engine(engine), mouseMoved(true) {}

Input::~Input() {
}

void Input::update(float dt) {
	//if (!mouseMoved && events.size() == 0) return;
	//mouseMoved = false;

	Interactable* interactable = nullptr;

	EntityHandle entity = getEntityAtPos(mousePosition.x, mousePosition.y);
	if (entity.isValid()) {
		interactable = entity.getComponent<Button>();
	}
	// Check for hover in/out update
	if (interactable != hoverPtr) {
		if (hoverPtr)
			hoverPtr->onHoverout();
		// Switch current hover ptr
		hoverPtr = interactable;

		if (hoverPtr)
			hoverPtr->onHoverover();
	}
	
	// Process input events
	for (InputEvent& event : events) {
		processInputEvent(event, interactable);
	}

	// Clear input events
	events.clear();
}
/* Process input event */
void Input::processInputEvent(const InputEvent& event, Interactable* interactable) const {
	switch (event.type) {
	case INPUT_EVENT_KEY:
		if (event.key.action == GLFW_PRESS) {
			keyPressed(event.key);
		}
		else if (event.key.action == GLFW_RELEASE) {
			keyReleased(event.key);
		}
		break;
	case INPUT_EVENT_MOUSEBUTTON: //TODO: ADD SHIFT-CLICK, CTRL-CLICK, ALT-CLICK
		// TODO: ADD MouseListener
		if (interactable && event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_LEFT) {
			if (event.mouseButton.action == GLFW_PRESS) {
				float clickTime = (float)glfwGetTime() - interactable->getTimeSinceLastClick();
				if (clickTime > DOUBLE_CLICK_THRESHOLD) {
					interactable->onLeftClickPressed();
					interactable->setTimeSinceLastClick((float)glfwGetTime());
				}
				else {
					interactable->onDoubleClick();
					interactable->setTimeSinceLastClick(0.0f);
				}
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				interactable->onLeftClickReleased();
			}
		} else if (interactable && event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_RIGHT) {
			if (event.mouseButton.action == GLFW_PRESS) {
				interactable->onRightClickPressed();
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				interactable->onRightClickReleased();
			}
		}
		else if (interactable && event.mouseButton.buttoncode == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (event.mouseButton.action == GLFW_PRESS) {
				interactable->onMiddleClickPressed();
			}
			else if (event.mouseButton.action == GLFW_RELEASE) {
				interactable->onMiddleClickReleased();
			}
		}
		break;
	case INPUT_EVENT_SCROLL:
		if (interactable) {
			interactable->onScroll(event.scroll.xoffset, event.scroll.yoffset);
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

// ------------------------------- CALLBACK -----------------------------------

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// Escape key is by default set for closing the application
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024)
	{
		Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

		InputEvent event;
		event.type			= INPUT_EVENT_KEY;
		event.key.keycode	= key;
		event.key.scancode	= scancode;
		event.key.action	= action;
		event.key.mode		= mode;

		input->addInputEvent(event);
	}
}

void Input::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
	input->setMousePos(glm::vec2(xpos, ypos));
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	InputEvent event;
	event.type						= INPUT_EVENT_MOUSEBUTTON;
	event.mouseButton.buttoncode	= button;
	event.mouseButton.action		= action;
	event.mouseButton.mods			= mods;

	input->addInputEvent(event);
}

void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	InputEvent event;
	event.type				= INPUT_EVENT_SCROLL;
	event.scroll.xoffset	= (float)xoffset;
	event.scroll.yoffset	= (float)yoffset;

	input->addInputEvent(event);
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