#ifndef HANDLE_ENTRY_H
#define HANDLE_ENTRY_H

#include <stdint.h>

namespace GameEngine {
	class Handle;

	class HandleEntry {
	public:
		HandleEntry();
		HandleEntry(uint32_t nextFreeIndex);
		~HandleEntry();

		const uint32_t getNextFreeIndex() const;
		const uint32_t getCounter() const;
		void* getEntry() const;
		const bool isActivated() const;
		const bool isEndOfList() const;

		void addToCounter();
		void setCounter(uint32_t count);
		void setActive(bool active);
		void setEntry(void* entry);
		void setEndOfList(bool end);
		void setNextFreeIndex(uint32_t nextFreeIndex);

	private:
		uint32_t nextFreeIndex : 12;
		uint32_t counter : 15;
		bool active;
		bool endOfList;

		void* entry;
	};
}
#endif