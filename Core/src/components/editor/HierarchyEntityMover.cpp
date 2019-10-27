#include "HierarchyEntityMover.h"
#include "HierarchySceneMover.h"
#include "HierarchyOrderRect.h"
#include "input/Input.h"
#include "graphics/Camera.h"
#include "graphics/data/Color.h"
#include "utils/Alignment.h"
#include "maths/MatrixTransform.h"
#include "HierarchyView.h"
#include "components/RectTransform.h"
#include "components/graphics/RectSprite.h"
#include "components/entity/ChildManager.h"
using namespace Core;

const float HierarchyEntityMover::START_DRAG_DISTANCE = 10;

HierarchyEntityMover::HierarchyEntityMover(ComponentHandle hierarchy, EntityHandle target) : hierarchy(hierarchy), target(target) {}
HierarchyEntityMover::~HierarchyEntityMover() {}



void HierarchyEntityMover::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	startPos = input->getMousePosition();
}

void HierarchyEntityMover::onMouseButtonReleased(int buttoncode, int mods) {
	// Hide graphics
	if (dragging) {
		dragging = false;
		const Vector2& mousePos = input->getMousePosition();
		EntityHandle drop = input->getEntityAtPos(mousePos.x, mousePos.y, std::vector<Entity>{graphics.getEntity()});
		destroyEntity(graphics);
		if (drop == owner) return;
		if (HierarchyEntityMover* mover = drop.getComponent<HierarchyEntityMover>()) {
			if (target == mover->target || mover->target.isParent(target.getEntity()) || target.getParent() == mover->target || target.getScene() != mover->target.getScene()) return;
			target.setParent(mover->target);
		}
		else if (HierarchySceneMover* mover = drop.getComponent<HierarchySceneMover>()) {
			target.removeParent();
			if (mover->scene != target.getScene()) {
				// TODO: Change scene
			}
		}
		else if (HierarchyOrderRect* orderRect = drop.getComponent<HierarchyOrderRect>()) {
			if (target != orderRect->entity)
				setOrder(target, orderRect->entity, orderRect->order);
		}
	}
}
void HierarchyEntityMover::onDestroy() {
	destroyEntity(graphics);
}

void HierarchyEntityMover::onMouseDrag(float mouseX, float mouseY) {
	if (dragging) {
		// Move graphics
		if (RectTransform* rect = graphics.getComponent<RectTransform>()) {
			Vector2 pos = camera->getViewMatrix() * input->getMousePosition();
			rect->setPosition(pos);
		}
	}
	else if (maths::distance(startPos, input->getMousePosition()) >= START_DRAG_DISTANCE) {
		dragging = true;
		RectTransform* rect = owner.getComponent<RectTransform>();
		if (rect) {
			Vector2 pos = input->getMousePosition();
			graphics = createEntity(owner.getEntityName() + "_Drag_Graphics",
				RectSprite(Color(255, 255, 255, 100)),
				RectTransform(pos.x, pos.y, rect->getSize().x, rect->getSize().y, rect->getZ() + 0.001f, Alignment::CENTER)
			);
		}
	}
}

bool HierarchyEntityMover::setOrder(Handle entity, EntityHandle target, std::size_t order) {
	// Check for parent switch
	if (target == entity) return false;
	if (target.isValid() && target.getChildCount() == 0) {
		target = target.getParent();
		order++;
	}
	else order = 0;
	EntityHandle currentParent = entity.getParent();
	std::vector<Handle> rootEntities = entity.getScene()->getRootEntities();
	std::size_t currentOrder = entity.getSiblingIndex();
	if (currentParent != target) {
		if (!target.isValid()) {
			entity.removeParent();
		}
		else if (entity != target) {
			entity.setParent(target);
		}
	}
	else if (target.isValid()) {
		// Entity
		ChildManager* childManager = target.getComponent<ChildManager>();
		if (childManager) {
			if (currentOrder < order) order--;
			if (order > childManager->getChildCount()) order = childManager->getChildCount();
			entity.setSiblingIndex(order);
			return true;
		}
		return false;
	}
	else {
		// Scene
		hierarchy.getComponent<HierarchyView>()->setRootIndex(entity, order);
	}
	return false;
}