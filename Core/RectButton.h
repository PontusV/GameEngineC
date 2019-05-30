#ifndef RECT_BUTTON_H
#define RECT_BUTTON_H
#include "Behaviour.h"
#include "Color.h"
#include "ComponentFunctionHandle.h"
#include <memory>
namespace Core {

	class RectButton : public Behaviour {
	public:
		enum ButtonState {
			DEFAULT = 0,
			PRESSED_DOWN,
			HOVER_OVER
		};
	public:
		RectButton(unsigned char layerIndex);
		RectButton() {}
		~RectButton();

		void awake() override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleasedAsButton(int buttoncode, int mods) override;
		void onHoverover() override;
		void onHoverout() override;

		void setLayerIndex(unsigned char value);

	private:
		void changeState(ButtonState state);

	public:
		Color colors[3];
		ComponentFunctionHandleWrapper clickFunction;
	private:
		unsigned char layerIndex;

		ButtonState state;
	};
}
#endif