#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#define DOUBLE_CLICK_THRESHOLD 0.2f

#include "Sprite.h"
#include "RectTransform.h"
#include "ComponentGroup.h"
#include "ComponentArray.h"

#include "EntityHandle.h"
#include "InputEvent.h"

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace Core {
	class Engine;
	class KeyListener;

	// Component Group for getEntityAtPos(x,y)
	struct SpriteGroup : public ComponentGroup<Sprite, RectTransform> {
		ComponentArray<Sprite>&			sprites		= getComponentArray<Sprite>();
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
	};

	class Input {
	public:
		Input(Engine* engine);
		~Input();

		void update(float dt);
		void addInputEvent(const InputEvent& event);

		//Observer
		void addKeyListener(KeyListener* listener);
		void addKeyBind(int keyCode, std::string buttonName);

		// Helper
		void setMousePosition(glm::vec2 position);
		/* Returns mouse position in screen space. */
		const glm::vec2& getMousePosition() const;

		EntityHandle getHoverTarget();
		EntityHandle getLastClicked();

	private:
		EntityHandle getEntityAtPos(float x, float y);
		void processInputEvent(const InputEvent& event, EntityHandle& target);

		// KEY
		void keyPressed(const KeyEvent& event) const;
		void keyReleased(const KeyEvent& event) const;
		std::string getButtonName(int keycode) const;

		// MOUSEBUTTON
		void mouseButtonPressed(const MouseButtonEvent& event) const;
		void mouseButtonReleased(const MouseButtonEvent& event) const;

	private:
		Engine* engine;
		std::vector<KeyListener*> keyListeners;
		std::map<int, std::string> keyBinds;

		// Input cache
		std::vector<InputEvent> events;

		// Interactable Component Groups
		SpriteGroup	spriteGroup;

		//TODO: List of contexts, A context contains a list of keybinds.
		//Input types: Actions (happens once on either buttonDown or buttonUp), States, Ranges
		//std::map<int, std::string> keyBindsPressed;
		//std::map<int, std::string> keyBindsReleased;

	private:
		glm::vec2 mousePosition;		// Mouse position in screen space
		bool mouseMoved;
		EntityHandle hoverTarget;
		EntityHandle lastClickTarget;
		float timeSinceLastClick;
		bool leftMouseButtonPressed;
	};
}
#endif