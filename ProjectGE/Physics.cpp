#include "Physics.h"
#include "BoxComponent.h"
using namespace Core;



Physics::Physics() {
}


Physics::~Physics() {
}

void onTransformChange(Transform& transform) {
	// Updates bounds
	std::vector<BoxComponent*> boxComponents = transform.getOwner().getComponents<BoxComponent>();
	for (BoxComponent* box : boxComponents) {
		box->updateBounds();
	}
}

/* Updates the Model Matrix of the target Transform and its child transforms.
 * @parameter matrixChanged specifies if the modelMatrix has changed from the previous update
*/
void updateTransformModel(Transform& root, glm::mat4 modelMatrix, bool matrixChanged) {
	if (root.hasChanged())
		matrixChanged = true;

	if (matrixChanged) {
		root.updateLocalToWorldMatrix(modelMatrix);
		onTransformChange(root);
	}
	modelMatrix = root.getLocalToWorldMatrix();

	// Iterate through all children (immediate children iterate through their immediate children)
	std::size_t childCount = root.getOwner().getImmediateChildCount();
	for (std::size_t childIndex = 0; childIndex < childCount; childIndex++) {
		Transform* childTransform = root.getOwner().getChild(childIndex).getComponent<Transform>();
		if (childTransform) {
			updateTransformModel(*childTransform, modelMatrix, matrixChanged);
		}
	}
}

void Physics::update(float dt) {
	std::size_t sizeTransform = rootTransforms.transforms.size();

	for (std::size_t i = 0; i < sizeTransform; i++) {
		Transform& root = rootTransforms.transforms[i];
		//root.rotate(60 * dt); // Tests

		//glm::mat4 rootModelMatrix = glm::mat4(1.0f);
		updateTransformModel(root, glm::mat4(1.0f), false);
	}
}