#include "Panel.h"
#include "GraphicComponent.h"
#include "ChildManager.h"
#include "Transform.h"

using namespace Core;


Panel::Panel(unsigned int width, unsigned int height) : BoxComponent(width, height) {
}

Panel::Panel() {
}

Panel::~Panel()
{
}

void Panel::init() {
	//childGraphics.registerListener(owner);
}
void Panel::end() {
	//childGraphics.unregisterListener(owner);
}

/*std::vector<GraphicComponent*> Panel::getChildGraphics() {
	return childGraphics.getCollection();
}*/

// ------------------------------- Serializable ----------------------------------------

void Panel::serialize(std::ostream& os) const {
	BoxComponent::serialize(os);
}

void Panel::deserialize(std::istream& is) {
	BoxComponent::deserialize(is);
}