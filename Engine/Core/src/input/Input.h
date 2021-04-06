#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#define DOUBLE_CLICK_THRESHOLD 0.2f

#include "components/graphics/SpriteRenderer.h"
#include "components/graphics/CanvasRenderer.h"
#include "components/RectTransform.h"
#include "components/ui/input/InputField.h"
#include "entity/component/ComponentGroup.h"
#include "entity/component/ComponentArray.h"

#include "entity/handle/EntityHandle.h"
#include "entity/handle/ComponentHandle.h"
#include "entity/handle/ComponentFunctionHandle.h"
#include "input/InputEvent.h"
#include "maths/Vector2.h"


#include <map>
#include <string>
#include <vector>
#include <chrono>

struct GLFWwindow;

namespace Core {
	class Engine;
	class BehaviourManager;
	class Graphics;
	class KeyListener;
	class Selectable;

	struct InputModifierFlags { // Same as GLFW https://www.glfw.org/docs/latest/group__mods.html#ga14994d3196c290aaa347248e51740274
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

		/* Handles all input events */
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
		/* Deselected the current target and selects the given target */
		void select(EntityHandle entity);
		/* Selects the next target as set by the currently selected target. */
		void selectNext();

		/* Sets the mouse position in screen space */
		void setMousePosition(Vector2 position);
		/* Returns mouse position in screen space. */
		const Vector2& getMousePosition() const;

		EntityHandle getHoverTarget();
		EntityHandle getLastLeftClicked();

		/* Gets the Handle of the Entity at the front at the given position in screen space. */
		EntityHandle getEntityHandleAtPos(float x, float y, std::vector<Entity> ignoreList = std::vector<Entity>());
		/* Gets the Entity at the front at the given position in screen space. */
		Entity getEntityAtPos(float x, float y);
	private:
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

	private:
		BehaviourManager& behaviourManager;
		Graphics& graphics;

		// Keyboard
		std::vector<KeyListener*> keyListeners;
		std::map<ButtonInput, Action> keyBinds; // buttoncode, Action
		std::vector<int> keysPressed;
		std::vector<int> keysDown;
		std::vector<int> keysReleased;
		TComponentHandle<InputField> keyboardFocus;

		// Others
		EntityHandle selectedTarget;
		static const float SELECT_NEXT_DELAY; // The delay before selecting next
		static const float INITIAL_SELECT_NEXT_DELAY; // The initial delay before repeating selectNext
		float timeBeforeNextSelect = 0.0f;
		bool selectNextHeldDown = false;

		// Input buffer
		std::vector<InputEvent> events;

		// Interactable Component Groups
		ComponentGroup<SpriteRenderer, RectTransform> spriteGroup;
		ComponentGroup<CanvasRenderer, RectTransform> guiGroup;

	private:
		Vector2 mousePosition;		// Mouse position in screen space
		bool mouseMoved;
		EntityHandle hoverTarget;
		EntityHandle lastLeftClickTarget;
		EntityHandle lastMiddleClickTarget;
		EntityHandle lastRightClickTarget;
		std::chrono::time_point<std::chrono::system_clock> timeSinceLastClick;
		bool leftMouseButtonPressed;
	};
}
#endif