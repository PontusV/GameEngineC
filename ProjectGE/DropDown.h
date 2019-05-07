#ifndef DROP_DOWN_H
#define DROP_DOWN_H
#include "Script.h"
#include "EntityHandle.h"
#include "Text.h"
#include "ComponentFunctionHandle.h"
#include "Font.h"
#include <cstddef>
namespace Core {

	struct DropDownOption {
		std::string text;
		ComponentFunctionHandleWrapper function;
	};

	/* Adds a Text and Rect component on start() */
	class DropDown : public Script {
	public:
		DropDown(Text text, int width, int height);
		DropDown();
		~DropDown();

		void	open();
		void	close();
		void	addOption(std::string label, ComponentFunctionHandleWrapper function);

		void	awake() override;
		void	update(float deltaTime) override;
		void	onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void	onMouseButtonPressed(int buttoncode, int mods) override;
		void	onMouseButtonReleased(int buttoncode, int mods) override;
		void	onHoverover() override;
		void	onHoverout() override;
		// State: Default, Hover, Open
		void	test();

	public:
		// Settings (set to Default values)
		int boxWidth = 200;
		int boxPaddingX = 5;
		int boxPaddingY = 5;
		int optionHeight = 20;
		Font optionFont = Font("resources/fonts/segoeui.ttf", 14);
		Color optionTextColor = { 255,255,255,255 };

	private:
		Text text;
		int width, height;
		bool isOpen;
		EntityHandle menuBox;
		std::vector<DropDownOption> options;
	};
}
#endif