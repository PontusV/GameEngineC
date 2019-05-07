#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "Anchor.h"
#include <glm/glm.hpp>

namespace Core {

	class Transform : public Component {
	public:
		Transform(float x, float y, float z = 0.0f, Anchor anchorPoint = Alignment::CENTER, float rotation = 0.0f, float scale = 1.0f);
		Transform();
		~Transform();

		void				rotate(float degrees);
		void				moveX(float value);
		void				moveY(float value);

		/* Pushes model matrix into worldModelMatrix */
		void				pushModelMatrix(const glm::mat4& model);
		void				updateLocalToWorldMatrix(const glm::mat4& model);
		void				resetLocalToWorldMatrix();
		const glm::mat4&	getLocalToWorldMatrix() const;
		const glm::mat4&	getWorldToLocalMatrix() const;
		/* Calculates and returns a new model matrix from local position, rotation and scale */
		glm::mat4			getLocalModelMatrix() const;

		/* Returns local rotation */
		const float&		getRotation() const;
		/* Returns local position */
		const glm::vec2&	getPosition() const;

		/* Returns local x coord */
		const float&		getX() const;
		/* Returns local y coord */
		const float&		getY() const;
		const float&		getZ() const;
		/* Sets local x coord */
		void				setX(float value);
		/* Sets local y coord */
		void				setY(float value);
		void				setZ(float value);

		bool				hasChanged();
		void				setAnchor(Anchor anchor);
		const glm::vec2&	getAnchor() const;

		glm::vec2			calculateOffset(const glm::vec2& size) const;

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	private:
		glm::mat4	localToWorldMatrix;		// World Model Matrix
		glm::mat4	worldToLocalMatrix;		// Inverse of worldModelMatrix. Multiply with a world position to get local position
		glm::vec2	position;				// Local position
		float		z;						// Depth for drawing order
		float		rotation;				// Local rotation in radians
		float		scale;					// Local scale (both width and height atm)

		glm::vec2	anchor;					// Determines where the position is relative to BoxComponents
		bool		changed = true;			// Defines if the transform has moved
	};
}
#endif