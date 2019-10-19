#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#define DOUBLE_CLICK_THRESHOLD 0.2f

#include "Sprite.h"
#include "RectTransform.h"
#include "InputField.h"
#include "GameObjectData.h"
#include "UIObjectData.h"
#include "ComponentGroup.h"
#include "ComponentArray.h"

#include "EntityHandle.h"
#include "ComponentHandle.h"
#include "ComponentFunctionHandle.h"
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

	class InputModifierFlags { // Same as GLFW https://www.glfw.org/docs/latest/group__mods.html#ga14994d3196c290aaa347248e51740274
		static constexpr int CTRL		= 0x0001;
		static constexpr int SHIFT		= 0x0002;
		static constexpr int ALT		= 0x0004;
		static constexpr int SUPER		= 0x0008;
		static constexpr int CAPS_LOCK	= 0x0010;
		static constexpr int NUM_LOCK	= 0x0020;
	};

	struct Action {
		static constexpr int RELEASE	= 0; // See GLFW_RELEASE
		static constexpr int PRESS		= 1; // See GLFW_PRESS
		Action() {} // TODO: Try to remove this since Action should not be default constructed
		Action(std::string name, int modFlags) : name(name) {}
		std::string name;
		std::map<int, std::vector<ComponentFunctionHandle<void>>> callbacks; // Action, function handle
	};

	struct ButtonInput {
		ButtonInput(int code, int modFlags = 0) : code(code), modFlags(modFlags) {}
		int code;
		int modFlags;
		bool operator<(const ButtonInput& other) const {
			if (code < other.code) return true;
			if (code > other.code) return false;
			return modFlags < other.modFlags;
		}
		bool operator==(const ButtonInput& other) const {
			return code == other.code && modFlags == other.modFlags;
		}
	};

	class Input {
	public:
		Input(Engine* engine);
		~Input();

		void update(float deltaTime);
		void addInputEvent(const InputEvent& event);

		/* Returns true if the key is held down. */
		bool getKeyDown(int keycode) const;
		/* Returns true if the key was pressed this frame. */
		bool getKeyPressed(int keycode) const;
		/* Returns true if the key was released this frame. */
		bool getKeyReleased(int keycode) const;
		/* Returns the character for the printable key, identified by the keycode. */
		std::string getKeyName(int keycode, int scancode = 0) const;

		/* Observer. Only used by Editor. */
		void addKeyListener(KeyListener* listener);
		/* Adds Action */
		void addAction(std::string actionName, int buttoncode, int modFlags = 0);
		/* Adds a callback function to be called when the Action is triggered. Action means for example Press or Release, etc... */
		void addActionCallback(std::string actionName, int action, ComponentFunctionHandle<void> function);
		/* Redirects all key input to the InputField. */
		void focusKeyboard(TComponentHandle<InputField> component);
		/* Removes focus for keyboard */
		void clearFocusKeyboard();
		/* Returns a handle to the keyboard focus */
		TComponentHandle<InputField> getKeyboardFocus();
		bool hasKeyboardFocus();

		/* Deselects the currently selected target. */
		void deselect();
		/* Deselected the current target and selects a new target */
		void select(EntityHandle entity, bool wasNext = false);
		/* Selects the next target as set by the currently selected target. */
		void selectNext();

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
		/* Process input and determines what Action to call. */
		void onKeyEvent(const KeyEvent& event);
		/* Marks the key as pressed and held down. */
		void keyPressed(const KeyEvent& event);
		/* Marks the key as released and not held down. */
		void keyReleased(const KeyEvent& event);
		/* Appends text to the current text written this frame. */
		void typeText(unsigned int codepoint);

		// MOUSEBUTTON
		void mouseButtonPressed(const MouseButtonEvent& event) const;
		void mouseButtonReleased(const MouseButtonEvent& event) const;

	private:
		Engine* engine;

		// Keyboard
		std::vector<KeyListener*> keyListeners;
		std::map<ButtonInput, Action> keyBinds; // buttoncode, Action
		std::vector<int> keysPressed;
		std::vector<int> keysDown;
		std::vector<int> keysReleased;
		TComponentHandle<InputField> keyboardFocus;

		// Others
		EntityHandle selectedTarget;
		const float SELECT_NEXT_DELAY = 0.05F; // The delay before selecting next
		const float INITIAL_SELECT_NEXT_DELAY = 0.5f; // The initial delay before repeating selectNext
		float timeBeforeNextSelect = 0.0f;
		bool selectNextHeldDown = false;

		// Input buffer
		std::vector<InputEvent> events;

		// Interactable Component Groups
		ComponentGroup<Sprite, RectTransform, GameObjectData> spriteGroup;
		ComponentGroup<Sprite, RectTransform, UIObjectData> spriteGroupUI;

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