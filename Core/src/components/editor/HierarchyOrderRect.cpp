#include "HierarchyOrderRect.h"
#include "components/graphics/RectSprite.h"
using namespace Core;


HierarchyOrderRect::HierarchyOrderRect(EntityHandle entity, std::size_t order) : entity(entity), order(order) {}
HierarchyOrderRect::~HierarchyOrderRect() {}

void HierarchyOrderRect::onMoveOut() {
	owner.getComponent<RectSprite>()->setColor(Color(0, 0, 0, 0));
}

void HierarchyOrderRect::onMoveOver() {
	owner.getComponent<RectSprite>()->setColor(Color(0, 0, 255, 120));
}