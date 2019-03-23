#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#define INPUT_EVENT_KEY				0
#define INPUT_EVENT_MOUSEBUTTON		1
#define INPUT_EVENT_SCROLL			2

namespace Core {
	struct KeyEvent {
		int keycode;
		int scancode;
		int action;
		int mode;
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
		KeyEvent			key;
		MouseButtonEvent	mouseButton;
		ScrollEvent			scroll;
	};
}
#endif