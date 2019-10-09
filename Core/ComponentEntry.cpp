#include "ComponentEntry.h"
using namespace Core;

ComponentEntry::ComponentEntry(EntityHandle owner, ComponentTypeID componentTypeID) : owner(owner), componentTypeID(componentTypeID) {}
ComponentEntry::~ComponentEntry() {}

void ComponentEntry::removeComponent() {
	owner.removeComponent(componentTypeID);
}