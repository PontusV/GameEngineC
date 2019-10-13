#ifndef BUTTON_H
#define BUTTON_H

#include "Behaviour.h"
#include "Image.h"
#include "Color.h"
#include "ComponentFunctionHandle.h"
#include <iostream>

namespace Core {

	#include "Button.generated.h"
	/* A button which manipulates the image component in the same Entity. A button does not function without an image. */
	CLASS() Button : public Behaviour {
		GENERATED_BODY()
	public:
		enum ButtonState {
			DEFAULT = 0,
			PRESSED_DOWN,
			HOVER_OVER,
			DISABLED
		};
	public:
		// Constructor / Destructor
		Button(Image defaultImage, Image pressedImage, Image hoverImage);
		Button(const char* defaultImage, const char* pressedImage, const char* hoverImage);
		Button() {}
		~Button();

		void awake() override;
		void onEnable() override;
		void onDisable() override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleasedAsButton(int buttoncode, int mods) override;
		void onHoverover() override;
		void onHoverout() override;

		Image* getImages();
	private:
		void changeState(ButtonState state);

	public:
		PROPERTY()
		Image images[4];
		PROPERTY()
		Color colors[4];
		PROPERTY()
		ComponentFunctionHandle<void> onLeftClick;
		PROPERTY()
		ComponentFunctionHandle<void> onMiddleClick;
		PROPERTY()
		ComponentFunctionHandle<void> onRightClick;

	private:
		ButtonState	state = ButtonState::DISABLED;
		bool leftPressed = false;
		bool middlePressed = false;
		bool rightPressed = false;
	};
}
#endif