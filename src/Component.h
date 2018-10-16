#ifndef COMPONENT_H
#define COMPONENT_H
#include "Serializable.h"
#include "Handle.h"

class Entity;
class Engine;

class Component : public Serializable {
public:
	virtual ~Component();
	void setID(Handle handleID);
	Handle getID() const;
	//Component type used for identifying different types of components while saving and loading
	enum componentTypes {
		Image, Text
	};
	Component& operator=(const Component& rhs)
	{
		//Do not change ID
		return *this;
	}
protected:
	Component();
private:
	Handle componentID;
};

std::istream& operator>> (std::istream& is, Component::componentTypes& e);
#endif