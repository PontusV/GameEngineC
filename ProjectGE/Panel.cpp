#include "Panel.h"
#include "GraphicComponent.h"
#include "ChildManager.h"
#include "Transform.h"
using namespace Core;


Panel::Panel(unsigned int width, unsigned int height) : BoxComponent(width, height) {
}

Panel::Panel(std::istream& is) {}


Panel::~Panel()
{
}

// Save & Load operator
void Panel::serialize(std::ostream& os) const {

}

void Panel::deserialize(std::istream& is) {

}

void Panel::init() {
	ChildManager* manager = owner.getComponent<ChildManager>();
	if (manager) {
		manager->registerListener(&graphicChildren);
	}

	transformHandle = ComponentHandle(owner, Transform::TYPE_ID);
}
void Panel::end() {
	// Remove soon to be invalid pointer from ChildManager
	ChildManager* manager = owner.getComponent<ChildManager>();
	if (manager) {
		manager->unregisterListener(&graphicChildren);
	}
}

std::vector<GraphicComponent*> Panel::getGraphicChildren() {
	return graphicChildren.getCollection();
}