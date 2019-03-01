#ifndef HANDLE_MANAGER_H
#define HANDLE_MANAGER_H

#include <stdint.h>
#include "HandleEntry.h"
#include "Handle.h"
#include <iostream>

/* Benefit of handles: These handles do not leave dangling pointers.
	Handles are a sort of ID
*/

namespace GameEngine {
	class HandleManager {
	public:
		static const int INVALID_HANDLE = 0;
		static const int HANDLE_TYPE_ENTITY = 1;
		static const int HANDLE_TYPE_COMPONENT = 2;
		static const int HANDLE_TYPE_COMPONENT_TRANSFORM = 3;
		static const int HANDLE_TYPE_COMPONENT_TEXT = 4;
		static const int HANDLE_TYPE_COMPONENT_IMAGE = 5;

		HandleManager();
		~HandleManager();

		void reset();
		Handle add(void* entry, uint32_t type);
		void update(Handle& handle, void* entry);
		void remove(const Handle& handle);

		template<typename T> T* get(Handle& handle) const;

		//Save & Load
		void write(std::ostream& os);
		void read(std::istream& is);
	private:
		static const int MAX_ENTRIES = 4096;
		HandleEntry entries[MAX_ENTRIES];

		int activeEntryCounter;
		uint32_t firstFreeEntry;
	};
	template<typename T>
	T* HandleManager::get(Handle& handle) const {
		const uint32_t index = handle.getIndex();
		if (entries[index].getCounter() == handle.getCounter() && entries[index].isActivated())
			return static_cast<T*>(entries[index].getEntry());
		return nullptr; //Handle is no longer valid
	}
}
#endif