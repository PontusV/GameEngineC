#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "HandleManager.h"
#include "glm/glm.hpp"

namespace Core {

	enum TransformAnchor {
		TOP_LEFT=0,		TOP,		TOP_RIGHT,
		LEFT,			CENTER,		RIGHT,
		BOTTOM_LEFT,	BOTTOM,		BOTTOM_RIGHT
	};

	class Transform : public Component {
	public:
		static constexpr ComponentTypeID TYPE_ID = 1;

		Transform(float x, float y, float z = 0.0f, TransformAnchor anchorPoint = TransformAnchor::CENTER, float rotation = 0.0f, float scale = 1.0f);
		Transform(std::istream& is);
		Transform();
		~Transform();
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		//Save & Load
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

		//Get & Set
		const float&		getRotation() const;
		const glm::vec2&	getPosition() const;

		const float&		getX() const;
		const float&		getY() const;
		const float&		getZ() const;
		void				setX(float y);
		void				setY(float x);
		void				setZ(float z);

		void				setAnchor(TransformAnchor anchor);
		const glm::vec2&	getAnchor() const;

		// Test
		glm::vec2			calculateOffset(const glm::vec2& size) const;
	private:
		glm::vec2	position;
		float		z;			//Depth for drawing order
		float		rotation;
		float		scale;

		glm::vec2	anchor; // Determines where the position is relative to the components
	};
}
#endif