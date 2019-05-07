#ifndef BUTTON_H
#define BUTTON_H

#include "Script.h"
#include "Image.h"
#include "Color.h"
#include <iostream>
#include <glm/glm.hpp>

namespace Core {

	/* A button which manipulates the image component in the same Entity. A button does not function without an image. */
	class Button : public Script {
	public:
		enum ButtonState {
			DEFAULT = 0,
			PRESSED_DOWN,
			HOVER_OVER
		};
	public:
		// Constructor / Destructor
		Button(Image defaultImage, Image pressedImage, Image hoverImage);																// Different sized images
		Button(const char* defaultImage, const char* pressedImage, const char* hoverImage, int width, int height);	// Same sized images
		Button() {}
		~Button();

		void	awake() override;
		void	onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void	onMouseButtonReleasedAsButton(int buttoncode, int mods) override;
		void	onHoverover() override;
		void	onHoverout() override;

		Image*	getImages();
		void	serialize(std::ostream& os) const;
		void	deserialize(std::istream& is);
	private:
		void	changeState(ButtonState state);

	public:
		Image		images[3];
		Color		colors[3];

	private:
		ButtonState	state;
	};
}
#endif