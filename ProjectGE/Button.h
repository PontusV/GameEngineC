#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "Interactable.h"
#include "Image.h"
#include "ComponentHandle.h"
#include <iostream>
#include <glm/glm.hpp>

namespace Core {

	enum ButtonState {
		DEFAULT = 0,
		PRESSED_DOWN,
		HOVER_OVER
	};

	/* A button which manipulates the image component in the same Entity. A button does not function without an image. */
	class Button : public Component, public ui::Interactable {
	public:
		static constexpr ComponentTypeID TYPE_ID = 4;
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		// Constructor / Destructor
		Button(Image defaultImage, Image pressedImage, Image hoverImage);																// Different sized images
		Button(const char* defaultImage, const char* pressedImage, const char* hoverImage, unsigned int width, unsigned int height);	// Same sized images
		Button() {}
		~Button();
		//
		void init();
		// Get
		Image* getImages();

		// UI
		void onLeftClickPressed() override;
		void onLeftClickReleased();
		void onHoverover();
		void onHoverout();

		// Callback
		void setButtonPressCallback(ui::LeftClickPressFun fun);
		void setButtonReleaseCallback(ui::LeftClickReleaseFun fun);

		// Serializable
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);
	private:
		// Changes state and image
		void changeState(ButtonState state);
	private:
		Image			images[3];
		ButtonState		state;

		// Ref to component with same owner
		ComponentHandle	imageHandle;
	};
}
#endif