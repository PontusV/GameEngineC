#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "Position.h"
#include "HandleManager.h"

namespace GameEngine {
	class Transform : public Component {
	public:
		static constexpr ComponentTypeID TYPE_ID = 1;
		static Transform* load(std::istream& is);

		Transform(float x, float y);
		Transform();
		~Transform();
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		//Save & Load
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

		//Get & Set
		float getX() const;
		float getY() const;
		void setX(float y);
		void setY(float x);
	private:
		Position position;
		//Rotation
		//Scale
	};
}
#include "TypeList.h"
typedef typename GameEngine::TypeList<TYPE_LIST, GameEngine::Transform>::type TypeListTransform;
#undef TYPE_LIST
#define TYPE_LIST TypeListTransform
#endif