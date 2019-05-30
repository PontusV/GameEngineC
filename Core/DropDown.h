#ifndef DROP_DOWN_H
#define DROP_DOWN_H
#include "Behaviour.h"
#include "EntityHandle.h"
#include "Text.h"
#include "ComponentFunctionHandle.h"
#include "Font.h"
#include <cstddef>
#include "DropDown.DropDown.generated.h"
namespace Core {

	struct DropDownOption {
		std::string text;
		ComponentFunctionHandleWrapper function;
	};

	/* Adds a Text and Rect component on start() */
	CLASS() DropDown : public Behaviour {
		GENERATED_BODY()
	public:
		DropDown(Text text);
		DropDown();
		~DropDown();

		void open();
		void close();
		void addOption(std::string label, ComponentFunctionHandleWrapper function);

		void awake() override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonPressed(int buttoncode, int mods) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;
		void onHoverover() override;
		void onHoverout() override;
		// State: Default, Hover, Open
		void test();

	public:
		// Settings (set to Default values)
		PROPERTY()
		int boxWidth = 200;
		PROPERTY()
		int boxPaddingX = 5;
		PROPERTY()
		int boxPaddingY = 5;
		PROPERTY()
		int optionHeight = 20;
		PROPERTY()
		Font optionFont = Font("resources/fonts/segoeui.ttf", 14);
		PROPERTY()
		Color optionTextColor = { 255,255,255,255 };

	private:
		PROPERTY()
		Text text;
		PROPERTY()
		std::vector<DropDownOption> options;
		bool isOpen = false;
		EntityHandle menuBox;
	};
}
#endif