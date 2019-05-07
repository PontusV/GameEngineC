#include "Panel.h"
#include "Sprite.h"
#include "ChildManager.h"
#include "Transform.h"

using namespace Core;


Panel::Panel(unsigned int width, unsigned int height) : BoxComponent(width, height) {
}

Panel::Panel() {
}

Panel::~Panel() {
}

// ------------------------------- Serializable ----------------------------------------

void Panel::serialize(std::ostream& os) const {
	BoxComponent::serialize(os);
}

void Panel::deserialize(std::istream& is) {
	BoxComponent::deserialize(is);
}