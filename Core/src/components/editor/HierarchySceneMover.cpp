#include "HierarchySceneMover.h"
#include "input/Input.h"
#include "graphics/Camera.h"
#include "graphics/data/Color.h"
#include "utils/Alignment.h"
#include "maths/MatrixTransform.h"
#include "HierarchyView.h"
#include "components/RectTransform.h"
#include "components/graphics/RectSprite.h"
using namespace Core;

const float HierarchySceneMover::START_DRAG_DISTANCE = 10;

HierarchySceneMover::HierarchySceneMover(ComponentHandle hierarchy, Scene* scene) : hierarchy(hierarchy), scene(scene) {}
HierarchySceneMover::~HierarchySceneMover() {}



void HierarchySceneMover::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	startPos = input->getMousePosition();
}

void HierarchySceneMover::onMouseButtonReleased(int buttoncode, int mods) {
	// Hide graphics
	if (dragging) {
		dragging = false;
		destroyEntity(graphics); // TODO
		/*const Vector2& mousePos = input->getMousePosition();
		EntityHandle drop = input->getEntityAtPos(mousePos.x, mousePos.y, std::vector<Entity>{graphics.getEntity()});
		destroyEntity(graphics);
		if (drop == owner) return;
		if (HierarchySceneMover * mover = drop.getComponent<HierarchySceneMover>()) {
			// TODO: Make Entities able to switch Scene
			if (target == mover->target || mover->target.isParent(target.getEntity()) || target.getParent() == mover->target || target.getScene() != mover->target.getScene()) return;
			target.setParent(mover->target);
		}*/
	}
}
void HierarchySceneMover::onDestroy() {
	destroyEntity(graphics);
}

void HierarchySceneMover::onMouseDrag(float mouseX, float mouseY) {
	if (dragging) {
		// Move graphics
		if (RectTransform * rect = graphics.getComponent<RectTransform>()) {
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