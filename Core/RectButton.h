#ifndef RECT_BUTTON_H
#define RECT_BUTTON_H
#include "Behaviour.h"
#include "Color.h"
#include "ComponentFunctionHandle.h"
#include <memory>
#include "RectButton.RectButton.generated.h"
namespace Core {
	CLASS() RectButton : public Behaviour {
		GENERATED_BODY()
	public:
		enum ButtonState {
			DEFAULT = 0,
			PRESSED_DOWN,
			HOVER_OVER
		};
	public:
		RectButton();
		~RectButton();

		void awake() override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleasedAsButton(int buttoncode, int mods) override;
		void onHoverover() override;
		void onHoverout() override;

	private:
		void changeState(ButtonState state);

	public:
		PROPERTY()
		Color colors[3];
		PROPERTY()
		ComponentFunctionHandle<> clickFunction;

	private:
		//PROPERTY() // ENUM!
		ButtonState state = ButtonState::DEFAULT;
	};
}
#endif