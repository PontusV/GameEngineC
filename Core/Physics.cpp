#include "Physics.h"
#include "BoxComponent.h"
#include "Behaviour.h"
#include "Text.h"
#include "ScrollRect.h"
#include "Maths/MatrixTransform.h"
#include "Maths/Matrix4.h"
using namespace Core;



Physics::Physics() {
}


Physics::~Physics() {
}

void onTransformChange(Transform& transform) {
	EntityHandle owner = transform.getOwner();
	// Updates bounds
	for (BoxComponent* box : owner.getComponents<BoxComponent>()) {
		box->updateBounds();
	}
	// Updates text position
	for (Text* text : owner.getComponents<Text>()) {
		text->updateTransforms();
	}
}

/* Updates the Model Matrix of the target Transform and its child transforms.
 * @parameter matrixChanged specifies if the modelMatrix has changed from the previous update
*/
void updateTransformModel(Transform& root, Matrix4 modelMatrix, bool matrixChanged) {
	if (root.hasChanged())
		matrixChanged = true;

	if (matrixChanged) {
		root.updateLocalToWorldMatrix(modelMatrix);
		onTransformChange(root);
	}

	// Iterate through all children (immediate children iterate through their immediate children)
	EntityHandle owner = root.getOwner();
	std::size_t childCount = owner.getImmediateChildCount();
	if (childCount > 0) {
		modelMatrix = root.getLocalToWorldMatrix() * root.getLocalModelMatrix();
		// Check for scrollrect
		if (ScrollRect* scrollRect = owner.getComponent<ScrollRect>()) {
			if (scrollRect->hasChanged() || matrixChanged) {
				modelMatrix = maths::translate(modelMatrix, Vector3(scrollRect->offset.x, scrollRect->offset.y, 0.0f));
				matrixChanged = true;
			}
		}
	}

	for (std::size_t childIndex = 0; childIndex < childCount; childIndex++) {
		Transform* childTransform = owner.getChild(childIndex).getComponent<Transform>();
		if (childTransform) {
			updateTransformModel(*childTransform, modelMatrix, matrixChanged);
		}
	}
	// Notify behaviours of change in transform
	if (matrixChanged) {
		for (Behaviour* behaviour : owner.getComponents<Behaviour>()) {
			behaviour->onTransformChanged();
		}
	}
}

void Physics::update(float dt) {
	std::size_t sizeTransform = rootTransforms.size();

	for (std::size_t i = 0; i < sizeTransform; i++) {
		Transform& root = rootTransforms.get<Transform>(i);

		//glm::mat4 rootModelMatrix = glm::mat4(1.0f);
		updateTransformModel(root, Matrix4(1.0f), false);
	}
}