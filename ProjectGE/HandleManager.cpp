#include "HandleManager.h"
#include "Handle.h"
#include <vector>
using namespace Core;



HandleManager::HandleManager() {
	reset();
}


HandleManager::~HandleManager() {
}

void HandleManager::reset() {
	activeEntryCounter = 0;
	firstFreeEntry = 0;

	for (int i = 0; i < MAX_ENTRIES - 1; i++)
		entries[i] = HandleEntry(i+1);
	entries[MAX_ENTRIES - 1] = HandleEntry();
}

Handle HandleManager::add(void* entry, uint32_t type) {
	if (activeEntryCounter >= MAX_ENTRIES)
		throw "HandleManager is full! Unable to add any additional entries. ";

	activeEntryCounter++;
	const uint32_t newIndex = firstFreeEntry;

	firstFreeEntry = entries[newIndex].getNextFreeIndex();
	entries[newIndex].addToCounter();
	entries[newIndex].setNextFreeIndex(0);
	entries[newIndex].setEntry(entry);
	entries[newIndex].setActive(true);

	return Handle(newIndex, entries[newIndex].getCounter(), type);
}

/* Makes the HandleEntry point to the given location */
void HandleManager::update(Handle& handle, void* entry) {
	const uint32_t index = handle.getIndex();
	if (entries[index].getCounter() == handle.getCounter() && entries[index].isActivated())
		entries[index].setEntry(entry);
}

void HandleManager::remove(const Handle& handle) {
	const uint32_t index = handle.getIndex();
	if (entries[index].getCounter() == handle.getCounter() && entries[index].isActivated()) {
		entries[index].setNextFreeIndex(firstFreeEntry);
		entries[index].setActive(false);
		firstFreeEntry = index;
		activeEntryCounter--;
	}
}