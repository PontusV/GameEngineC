#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#define DOUBLE_CLICK_THRESHOLD 0.2f

#include "Sprite.h"
#include "RectTransform.h"
#include "GameObjectData.h"
#include "UIObjectData.h"
#include "ComponentGroup.h"
#include "ComponentArray.h"

#include "EntityHandle.h"
#include "InputEvent.h"
#include "Maths/Vector2.h"

#include <map>
#include <string>
#include <vector>

struct GLFWwindow;

namespace Core {
	class Engine;
	class KeyListener;
	class Selectable;

	class Input {
	public:
		Input(Engine* engine);
		~Input();

		void update(float dt);
		void addInputEvent(const InputEvent& event);

		/* Returns true if the key is held down. */
		bool getKeyDown(int keycode) const;
		/* Returns true if the key was pressed this frame. */
		bool getKeyPressed(int keycode) const;
		/* Returns true if the key was released this frame. */
		bool getKeyReleased(int keycode) const;
		/* Returns a vector of all keys currently being held down. */
		std::vector<int> getKeysDown();
		/* Returns a vector of all keys that got pressed this frame. */
		std::vector<int> getKeysPressed();
		/* Returns a vector of all keys that got released this frame. */
		std::vector<int> getKeysReleased();
		/* Returns the character for the printable key, identified by the keycode. */
		std::string getKeyName(int keycode, int scancode = 0) const;
		/* Returns a string of the text typed this frame. */
		const std::wstring& getTextTyped() const;

		//Observer
		void addKeyListener(KeyListener* listener);
		void addKeyBind(int keyCode, std::string buttonName);

		// Helper
		void setMousePosition(Vector2 position);
		/* Returns mouse position in screen space. */
		const Vector2& getMousePosition() const;

		EntityHandle getHoverTarget();
		EntityHandle getLastLeftClicked();

	private:
		EntityHandle getEntityAtPos(float x, float y);
		void processInputEvent(const InputEvent& event, EntityHandle& target);

		// KEY
		void keyPressed(const KeyEvent& event);
		void keyReleased(const KeyEvent& event);
		std::string getButtonName(int keycode) const;
		void typeText(unsigned int codepoint);

		// MOUSEBUTTON
		void mouseButtonPressed(const MouseButtonEvent& event) const;
		void mouseButtonReleased(const MouseButtonEvent& event) const;

	private:
		Engine* engine;
		std::vector<KeyListener*> keyListeners;
		std::map<int, std::string> keyBinds;
		std::vector<int> keysPressed;
		std::vector<int> keysDown;
		std::vector<int> keysReleased;
		std::wstring typedText; // contains the text typed this frame
		std::vector<Selectable*> selectedScripts;

		// Input cache
		std::vector<InputEvent> events;

		// Interactable Component Groups
		ComponentGroup<Sprite, RectTransform, GameObjectData> spriteGroup;
		ComponentGroup<Sprite, RectTransform, UIObjectData> spriteGroupUI;

		//TODO: List of contexts, A context contains a list of keybinds.
		//Input types: Actions (happens once on either buttonDown or buttonUp), States, Ranges
		//std::map<int, std::string> keyBindsPressed;
		//std::map<int, std::string> keyBindsReleased;

	private:
		Vector2 mousePosition;		// Mouse position in screen space
		bool mouseMoved;
		EntityHandle hoverTarget;
		EntityHandle lastLeftClickTarget;
		EntityHandle lastMiddleClickTarget;
		EntityHandle lastRightClickTarget;
		float timeSinceLastClick;
		bool leftMouseButtonPressed;
	};
}
#endif