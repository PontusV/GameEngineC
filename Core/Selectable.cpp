#include "Selectable.h"
#include <GLFW/glfw3.h>
using namespace Core;


Selectable::Selectable()
{
}


Selectable::~Selectable()
{
}


void Selectable::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (buttoncode == GLFW_MOUSE_BUTTON_1) {
		onSelect();
		selected = true;
	}
}

void Selectable::onMouseButtonPressed(int buttoncode, int mods) {
	if (buttoncode == GLFW_MOUSE_BUTTON_1) {
		onDeselect();
		selected = false;
	}
}

bool Selectable::isSelected() {
	return selected;
}