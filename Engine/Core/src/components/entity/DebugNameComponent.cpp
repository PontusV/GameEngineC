#include "DebugNameComponent.h"
using namespace Core;

DebugNameComponent::DebugNameComponent(const char* name) : name(name) {}
DebugNameComponent::DebugNameComponent() {}
DebugNameComponent::~DebugNameComponent() {}

void DebugNameComponent::setName(std::string name) {
	this->name = name;
}

std::string DebugNameComponent::getName() const {
	return name;
}