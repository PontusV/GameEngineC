#include "Physics.h"
#include "components/Behaviour.h"
#include "components/graphics/Text.h"
#include "components/ui/ScrollRect.h"
#include "maths/MatrixTransform.h"
#include "maths/Matrix4.h"
using namespace Core;



Physics::Physics(EntityManager* entityManager) : entityManager(entityManager) {
}


Physics::~Physics() {
}

void onTransformChange(Transform& transform, Handle& owner) {
	// Updates text position
	for (Text* text : owner.getComponents<Text>()) {
		text->setDirty();
	}
}

/* Updates the Model Matrix of the target Transform and its child transforms.
 * @parameter matrixChanged specifies if the modelMatrix has changed from the previous update
*/
void updateTransformModel(Transform& root, Handle& owner, Matrix4 modelMatrix, bool matrixChanged) {
	bool blockNotify = false;
	if (root.hasChanged()) {
		matrixChanged = true;
		root.updateLocalModelMatrix();
	}

	if (matrixChanged) {
		root.updateLocalToWorldMatrix(modelMatrix);
		onTransformChange(root, owner);
	}

	// Iterate through all children (immediate children iterate through their immediate children)
	std::size_t childCount = owner.getImmediateChildCount();
	if (childCount > 0) {
		modelMatrix = root.getLocalToWorldMatrix() * root.getLocalModelMatrix();
		// Check for scrollrect
		if (ScrollRect* scrollRect = owner.getComponent<ScrollRect>()) {
			if (scrollRect->hasChanged() || matrixChanged) {
				modelMatrix = maths::translate(modelMatrix, Vector3(scrollRect->offset.x, scrollRect->offset.y, 0.0f));
				if (!matrixChanged) blockNotify = true;
				matrixChanged = true;
			}
		}
	}

	for (std::size_t childIndex = 0; childIndex < childCount; childIndex++) {
		Handle child = owner.getChild(childIndex);
		Transform* childTransform = child.getComponent<Transform>();
		if (childTransform) {
			updateTransformModel(*childTransform, child, modelMatrix, matrixChanged);
		}
	}
	// Notify behaviours of change in transform
	if (!blockNotify && matrixChanged) {
		for (Behaviour* behaviour : owner.getComponents<Behaviour>()) {
			behaviour->onTransformChanged();
		}
	}
}

void Physics::update(float dt) {
	std::size_t sizeTransform = rootTransforms.size();

	for (std::size_t i = 0; i < sizeTransform; i++) {
		Transform& root = rootTransforms.get<Transform>(i);
		Handle owner = rootTransforms.createHandle(i, entityManager);

		//glm::mat4 rootModelMatrix = glm::mat4(1.0f);
		updateTransformModel(root, owner, Matrix4(1.0f), false);
	}
}