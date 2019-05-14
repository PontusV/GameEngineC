#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

namespace Core {

	class Transform : public Component {
	public:
		Transform(float x, float y, float z = 0.0f, float rotation = 0.0f, float scale = 1.0f);
		Transform();
		virtual ~Transform();

		void rotate(float degrees);
		void moveX(float value);
		void moveY(float value);

		/* Pushes model matrix into worldModelMatrix */
		void pushModelMatrix(const glm::mat4& model);
		void updateLocalToWorldMatrix(const glm::mat4& model);
		void resetLocalToWorldMatrix();
		const glm::mat4& getLocalToWorldMatrix() const;
		const glm::mat4& getWorldToLocalMatrix() const;
		/* Calculates and returns a new model matrix from local position, rotation and scale */
		glm::mat4 getLocalModelMatrix() const;

		/* Returns local rotation */
		const float& getLocalRotation() const;
		/* Returns position in local space. */
		const glm::vec2& getLocalPosition() const;
		/* Returns position in world space. */
		const glm::vec2 getPosition() const;

		const float& getZ() const;
		/* Sets local x coord */
		void setX(float value);
		/* Sets local y coord */
		void setY(float value);
		void setZ(float value);

		bool hasChanged();

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	protected:
		glm::mat4 localToWorldMatrix;	// World Model Matrix
		glm::mat4 worldToLocalMatrix;	// Inverse of worldModelMatrix. Multiply with a world position to get local position
		glm::vec2 position;				// Local position
		float z;						// Depth for drawing order
		float rotation;					// Local rotation in radians
		float scale;					// Local scale (both width and height atm)

		bool changed = true;			// Defines if the transform has moved
	};
}
#endif