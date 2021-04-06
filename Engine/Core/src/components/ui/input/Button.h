#ifndef BUTTON_H
#define BUTTON_H

#include "components/Behaviour.h"
#include "components/graphics/Image.h"
#include "graphics/data/Color.h"
#include "entity/handle/ComponentFunctionHandle.h"
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
		Button(Image defaultImage, Image pressedImage, Image hoverImage); // TODO: Add disabledImage option, default of disabled is default
		Button(const char* defaultImage, const char* pressedImage, const char* hoverImage); // TODO: Add disabledImage option, default of disabled is default
		Button() {}
		~Button();

		void awake() override;
		void onEnable() override;
		void onDisable() override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleasedAsButton(int buttoncode, int mods) override;
		void onHoverOver() override;
		void onHoverOut() override;

		Image* getImages();
	private:
		void changeState(ButtonState state);

	public:
		PROPERTY()
		Image images[4];
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