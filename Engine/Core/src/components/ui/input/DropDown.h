#ifndef DROP_DOWN_H
#define DROP_DOWN_H
#include "components/Behaviour.h"
#include "entity/handle/EntityHandle.h"
#include "components/graphics/Text.h"
#include "entity/handle/ComponentFunctionHandle.h"
#include "graphics/font/Font.h"
#include <cstddef>
#include "DropDown.generated.h"
namespace Core {

	struct DropDownOption {
		std::string text;
		ComponentFunctionHandle<void> function;
	};

	/* Adds a Text and Rect component on start() */
	CLASS() DropDown : public Behaviour {
		GENERATED_BODY()
	public:
		DropDown(Text text);
		DropDown();
		~DropDown();

		virtual void open();
		void close();
		void addOption(std::string label, ComponentFunctionHandle<void> function);

		void start() override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonPressed(int buttoncode, int mods) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;
		void onHoverOver() override;
		void onHoverOut() override;
		void onDestroy() override;
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
		Font optionFont = Font("Fonts/segoeui.ttf", 14);
		PROPERTY()
		Color optionTextColor = { 255,255,255,255 };
		PROPERTY()
		bool border = true;
		PROPERTY()
		int borderSize = 1;
		PROPERTY()
		Color borderColor = Color(100, 100, 100, 255);

	protected:
		PROPERTY()
		Text text;
		//PROPERTY()
		std::vector<DropDownOption> options;
		bool isOpen = false;
		EntityHandle menuBox;
	};
}
#endif