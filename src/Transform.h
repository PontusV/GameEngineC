#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "Position.h"
#include "HandleManager.h"


class Transform :
	public Component
{
public:
	static const int TYPE_ID = HandleManager::HANDLE_TYPE_COMPONENT_TRANSFORM;
	static Transform* load(std::istream& is);

	Transform(float x, float y);
	~Transform();

	//Save & Load
	void serialize(std::ostream& os) const;
	void deserialize(std::istream& is);

	//Get & Set
	float getX() const;
	float getY() const;
	void setX(float y);
	void setY(float x);
private:
	Transform();
	Position position;
	//Rotation
	//Scale
};
#endif