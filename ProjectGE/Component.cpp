#include "Component.h"
#include <iostream>
using namespace GameEngine;



Component::Component() {
}
Component::~Component() {
}


void Component::setID(Handle handleID) {
	componentID = handleID;
}

Handle Component::getID() const {
	return componentID;
}

/* Load component type (enum) */
//std::istream& operator>> (std::istream& is, Component::componentTypes& e) {
//	int i;
//	if (!(is >> i)) { /* Handle error */ }
//	e = static_cast<Component::componentTypes>(i);
//	return is;
//}