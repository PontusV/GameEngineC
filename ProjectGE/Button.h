#ifndef BUTTON_H
#define BUTTON_H

#include "Script.h"
#include "Image.h"
#include <iostream>
#include <glm/glm.hpp>

namespace Core {

	enum ButtonState {
		DEFAULT = 0,
		PRESSED_DOWN,
		HOVER_OVER
	};

	/* A button which manipulates the image component in the same Entity. A button does not function without an image. */
	class Button : public Script {
	public:
		// Constructor / Destructor
		Button(Image defaultImage, Image pressedImage, Image hoverImage);																// Different sized images
		Button(const char* defaultImage, const char* pressedImage, const char* hoverImage, unsigned int width, unsigned int height);	// Same sized images
		Button() {}
		~Button();

		void	start() override;
		void	onMouseButtonPressed(int buttoncode, int mods) override;
		void	onMouseButtonReleased(int buttoncode, int mods) override;
		void	onHoverover() override;
		void	onHoverout() override;

		Image*	getImages();
		void	serialize(std::ostream& os) const;
		void	deserialize(std::istream& is);
	private:
		// Changes state and image
		void	changeState(ButtonState state);
	private:
		Image			images[3];
		ButtonState		state;
	};
}
#endif