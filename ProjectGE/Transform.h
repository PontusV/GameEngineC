#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "Position.h"
#include "HandleManager.h"
#include "glm/glm.hpp"

namespace GameEngine {
	class Transform : public Component {
	public:
		static constexpr ComponentTypeID TYPE_ID = 1;

		Transform(float x, float y, float z = 0.0f, float rotation = 0.0f);
		Transform(std::istream& is);
		Transform();
		~Transform();
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		//Save & Load
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

		//Get & Set
		const float& getRotation() const;
		const glm::vec2& getPosition() const;

		float getX() const;
		float getY() const;
		float getZ() const;
		void setX(float y);
		void setY(float x);
		void setZ(float z);
	private:
		glm::vec2 position;
		float z; //Depth for drawing order
		float rotation;
		//Rotation
		//Scale
	};
}
#include "TypeList.h"
typedef typename GameEngine::TypeList<TYPE_LIST, GameEngine::Transform>::type TypeListTransform;
#undef TYPE_LIST
#define TYPE_LIST TypeListTransform
#endif