#include "HorizontalLayoutGroup.h"
#include "LayoutElement.h"

using namespace Core;

HorizontalLayoutGroup::HorizontalLayoutGroup() {
}


HorizontalLayoutGroup::~HorizontalLayoutGroup() {
}


void HorizontalLayoutGroup::update(float deltaTime) {
	std::vector<LayoutElement*> components = owner.getComponentsInChildren<LayoutElement>();
}