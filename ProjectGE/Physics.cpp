#include "Physics.h"
using namespace Core;



Physics::Physics() {
}


Physics::~Physics() {
}

/* Updates the Model Matrix of the target Transform and its child transforms.
 * @parameter matrixChanged specifies if the modelMatrix has changed from the previous update
*/
void updateTransformModel(Transform& root, glm::mat4 modelMatrix, bool matrixChanged) {
	if (!matrixChanged && root.hasChanged())
		matrixChanged = true;

	if (matrixChanged) {
		modelMatrix *= root.getLocalModelMatrix();
		root.updateWorldModelMatrix(modelMatrix);
	} else {
		modelMatrix = root.getWorldModelMatrix();
	}

	// Iterate through all children
	std::size_t childCount = root.getChildCount();
	for (std::size_t childIndex = 0; childIndex < childCount; childIndex++) {
		Transform* childTransform = root.getChild(childIndex)->getComponent<Transform>();
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

		glm::mat4 rootModelMatrix = glm::mat4(1.0f);
		updateTransformModel(root, rootModelMatrix, false);
	}
}