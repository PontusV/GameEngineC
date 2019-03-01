#include "HandleManager.h"
#include "Handle.h"
#include <vector>
using namespace GameEngine;



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
void HandleManager::write(std::ostream& os) {
	os.write((char*)&firstFreeEntry, sizeof(uint32_t));															//Save firstFreeEntry
	std::vector<HandleEntry> activeEntries;
	std::vector<int> indexes;
	for (int i = 0; i < MAX_ENTRIES; i++) {
		if (entries[i].isActivated()) { //Add active entries to vector
			activeEntries.push_back(entries[i]);
			indexes.push_back(i);
		}
	}
	int size = activeEntries.size();
	os.write((char*)&size, sizeof(int));															//Save amount of entries to be loaded
	for (unsigned int i = 0; i < activeEntries.size(); i++) {
		uint32_t nextFreeIndex = activeEntries.at(i).getNextFreeIndex();
		uint32_t counter = activeEntries.at(i).getCounter();

		os.write((char*)&nextFreeIndex, sizeof(uint32_t));													//Save next free index
		os.write((char*)&counter, sizeof(uint32_t));														//Save counter
		os.write((char*)&indexes.at(i), sizeof(int));														//Save index
	}
}
void HandleManager::read(std::istream& is) {
	is.read((char*)&firstFreeEntry, sizeof(uint32_t));															//Load firstFreeEntry
	int amount; //Amount of entries to be loaded
	is.read((char*)&amount, sizeof(int));																		//Load amount of entries to be loaded
	for (int i = 0; i < amount; i++) {
		uint32_t nextFreeIndex, counter;
		int index;
		is.read((char*)&nextFreeIndex, sizeof(uint32_t));													//Load next free index
		is.read((char*)&counter, sizeof(uint32_t));															//Load counter
		//Pointer is loaded by entities with the update function
		is.read((char*)&index, sizeof(int));																	//Load index
		//Activate and set values of loaded entry
		entries[index].setActive(true);
		entries[index].setNextFreeIndex(nextFreeIndex);
		entries[index].setCounter(counter);
	}
}