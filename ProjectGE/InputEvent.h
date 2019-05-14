#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H


#define INPUT_EVENT_KEY				0
#define INPUT_EVENT_MOUSEBUTTON		1
#define INPUT_EVENT_SCROLL			2

#define MOUSE_BUTTON_LEFT			0
#define MOUSE_BUTTON_RIGHT			1
#define MOUSE_BUTTON_MIDDLE			2

#define INPUT_MOD_SHIFT				0x0001
#define INPUT_MOD_CONTROL			0x0002
#define INPUT_MOD_ALT				0x0004
#define INPUT_MOD_SUPER				0x0008
#define INPUT_MOD_CAPS_LOCK			0x0010
#define INPUT_MOD_NUM_LOCK			0x0020

namespace Core {
	struct KeyEvent {
		int keycode;
		int scancode;
		int action;
		int mods;
	};

	struct MouseButtonEvent {
		int buttoncode;
		int action;
		int mods;
	};

	struct ScrollEvent {
		float xoffset;
		float yoffset;
	};

	struct InputEvent {
		int type;
		KeyEvent key;
		MouseButtonEvent mouseButton;
		ScrollEvent scroll;
	};
}
#endif