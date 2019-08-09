#include "ReflectedObjectHandle.h"
#include "ReflectedObject.h"
using namespace Core;

bool ReflectedObjectHandle::isValid() {
	return handle.isValid();
}

ReflectedObject* ReflectedObjectHandle::get() {
	return reinterpret_cast<ReflectedObject*>(reinterpret_cast<char*>(handle.getComponent()) + offset);
}