#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

namespace Core {

	enum TransformAnchor {
		TOP_LEFT=0,		TOP,		TOP_RIGHT,
		LEFT,			CENTER,		RIGHT,
		BOTTOM_LEFT,	BOTTOM,		BOTTOM_RIGHT
	};

	class Transform : public Component {
	public:
		Transform(float x, float y, float z = 0.0f, TransformAnchor anchorPoint = TransformAnchor::CENTER, float rotation = 0.0f, float scale = 1.0f);
		Transform(float x, float y, float z = 0.0f, glm::vec2 anchorPoint = glm::vec2(0,0), float rotation = 0.0f, float scale = 1.0f);
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

		const float&		getRotation() const;
		const glm::vec2&	getPosition() const;

		const float&		getX() const;
		const float&		getY() const;
		const float&		getZ() const;
		void				setX(float value);
		void				setY(float value);
		void				setZ(float value);

		bool				hasChanged();
		void				setAnchor(TransformAnchor anchor);
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

		bool		changed;				// Defines if the transform has moved
		glm::vec2	anchor;					// Determines where the position is relative to BoxComponents
	};
}
#endif