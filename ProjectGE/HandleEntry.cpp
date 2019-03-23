#include "HandleEntry.h"
#include "Handle.h"
using namespace Core;


HandleEntry::HandleEntry() : nextFreeIndex(0), counter(0), active(false), endOfList(true), entry(nullptr) {}
HandleEntry::HandleEntry(uint32_t nextFreeIndex) : nextFreeIndex(nextFreeIndex), counter(0), active(false), endOfList(false), entry(nullptr) {}

HandleEntry::~HandleEntry() {}


void* HandleEntry::getEntry() const {
	return entry;
}

const uint32_t HandleEntry::getNextFreeIndex() const {
	return nextFreeIndex;
}

const bool HandleEntry::isActivated() const {
	return active;
}

const bool HandleEntry::isEndOfList() const {
	return endOfList;
}

const uint32_t HandleEntry::getCounter() const {
	return counter;
}

void HandleEntry::addToCounter() {
	counter++;
}

void HandleEntry::setActive(bool active) {
	this->active = active;
}

void HandleEntry::setEntry(void* entry) {
	this->entry = entry;
}

void HandleEntry::setEndOfList(bool end) {
	endOfList = end;
}

void HandleEntry::setNextFreeIndex(uint32_t nextFreeIndex) {
	this->nextFreeIndex = nextFreeIndex;
}

void HandleEntry::setCounter(uint32_t count) {
	counter = count;
}