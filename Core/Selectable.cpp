#include "Selectable.h"
#include <GLFW/glfw3.h>
using namespace Core;


Selectable::Selectable()
{
}


Selectable::~Selectable()
{
}


void Selectable::select() {
	selected = true;
	onSelect();
}
void Selectable::deselect() {
	selected = false;
	onDeselect();
}

bool Selectable::isSelected() {
	return selected;
}

void Selectable::setNext(EntityHandle entity) {
	nextSelectable = entity;
}

EntityHandle Selectable::getNext() {
	nextSelectable.refresh();
	return nextSelectable;
}