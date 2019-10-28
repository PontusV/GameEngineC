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

Handle HierarchyEntityMover::getEntityBelow() {
	const Vector2& mousePos = input->getMousePosition();
	return input->getEntityAtPos(mousePos.x, mousePos.y, std::vector<Entity>{graphics.getEntity()});
}

void HierarchyEntityMover::onMouseButtonReleased(int buttoncode, int mods) {
	// Hide graphics
	if (dragging) {
		dragging = false;
		hoverOut(entityBelow);
		entityBelow.clear();
		EntityHandle drop = getEntityBelow();
		destroyEntity(graphics);
		if (drop == owner) return;
		if (HierarchyEntityMover* mover = drop.getComponent<HierarchyEntityMover>()) {
			if (target == mover->target || mover->target.isParent(target.getEntity()) || target.getParent() == mover->target || target.getScene() != mover->target.getScene()) return;
			target.setParent(mover->target);
		}
		else if (HierarchySceneMover* mover = drop.getComponent<HierarchySceneMover>()) {
			if (mover->scene == target.getScene()) {
				target.removeParent();
			}
		}
		else if (HierarchyOrderRect* orderRect = drop.getComponent<HierarchyOrderRect>()) {
			if (target == orderRect->entity || orderRect->entity.isParent(target.getEntity()) || target.getParent() == orderRect->entity || target.getScene() != orderRect->entity.getScene()) return;
				setOrder(target, orderRect->entity, orderRect->order);
		}
	}
}
void HierarchyEntityMover::onDestroy() {
	destroyEntity(graphics);
}

void HierarchyEntityMover::hoverOut(EntityHandle entity) {
	if (HierarchyEntityMover* mover = entityBelow.getComponent<HierarchyEntityMover>()) {
		mover->onMoveOut();
	}
	else if (HierarchyOrderRect* orderRect = entityBelow.getComponent<HierarchyOrderRect>()) {
		orderRect->onMoveOut();
	}
}

void HierarchyEntityMover::hoverOver(EntityHandle entity) {
	if (HierarchyEntityMover* mover = entityBelow.getComponent<HierarchyEntityMover>()) {
		mover->onMoveOver();
	}
	else if (HierarchyOrderRect* orderRect = entityBelow.getComponent<HierarchyOrderRect>()) {
		orderRect->onMoveOver();
	}
}

void HierarchyEntityMover::onHoverOver() {
	onMoveOver();
}

void HierarchyEntityMover::onHoverOut() {
	onMoveOut();
}

void HierarchyEntityMover::onMoveOut() {
	owner.getComponent<RectSprite>()->setColor(Color(0, 0, 0, 0));
}

void HierarchyEntityMover::onMoveOver() {
	owner.getComponent<RectSprite>()->setColor(Color(255, 255, 255, 80));
}

void HierarchyEntityMover::onMouseDrag(float mouseX, float mouseY) {
	if (dragging) {
		// Move graphics
		if (RectTransform* rect = graphics.getComponent<RectTransform>()) {
			Vector2 pos = camera->getViewMatrix() * input->getMousePosition();
			rect->setPosition(pos);

			// TODO: Highlight order rects & Entries
			EntityHandle currentEntityBelow = getEntityBelow();
			if (currentEntityBelow != entityBelow) {
				hoverOut(entityBelow);
				entityBelow = getEntityBelow();
				hoverOver(entityBelow);
			}
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
	if (currentParent != target) {
		if (!target.isValid()) {
			entity.removeParent();
			hierarchy.getComponent<HierarchyView>()->setRootIndex(entity, order);
		}
		else if (entity != target) {
			entity.setParent(target);
			entity.setSiblingIndexQueued(order);
		}
	}
	else if (target.isValid()) {
		// Entity
		ChildManager* childManager = target.getComponent<ChildManager>();
		if (childManager) {
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