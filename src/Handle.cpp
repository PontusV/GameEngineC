#include "Handle.h"
#include "HandleManager.h"
using namespace GameEngine;


Handle::Handle() : index(-1), counter(-1), type(HandleManager::INVALID_HANDLE) {}

Handle::Handle(uint32_t index, uint32_t counter, uint32_t type) : index(index), counter(counter), type(type) {}
Handle::Handle(uint32_t values) {
	index = values;
	counter = values >> 12;
	type = values >> 27;
}

Handle::~Handle() {}


Handle::operator uint32_t() const {
	return type << 27 | counter << 12 | index;
}

const uint32_t Handle::getIndex() const {
	return index;
}

const uint32_t Handle::getCounter() const {
	return counter;
}

const uint32_t Handle::getType() const {
	return type;
}

void Handle::addToCounter() {
	counter++;
}