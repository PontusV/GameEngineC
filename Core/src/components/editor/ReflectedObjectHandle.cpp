#include "ReflectedObjectHandle.h"
using namespace Core;

bool ReflectedObjectHandle::isValid() {
	return handle.isValid();
}

void* ReflectedObjectHandle::get() {
	if (Component* component = handle.getComponent())
		return reinterpret_cast<void*>(reinterpret_cast<char*>(handle.getComponent()) + offset);
	return nullptr;
}

std::size_t ReflectedObjectHandle::getTypeID() const {
	return typeID;
}

Component* ReflectedObjectHandle::getRoot() {
	return handle.getComponent();
}

std::size_t ReflectedObjectHandle::getRootPropTypeID() {
	return handle.getComponent()->getType().typeID;
}