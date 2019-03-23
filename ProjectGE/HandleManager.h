#ifndef HANDLE_MANAGER_H
#define HANDLE_MANAGER_H

#include <stdint.h>
#include "HandleEntry.h"
#include "Handle.h"
#include <iostream>

/* Benefit of handles: These handles do not leave dangling pointers. */

namespace Core {
	class HandleManager {
	public:
		/*static HandleManager& getInstance() {
			static HandleManager instance;
			return instance;
		}*/
	public:
		static const int INVALID_HANDLE = 0;
		static const int HANDLE_TYPE_COMPONENT = 1;

		HandleManager();
		~HandleManager();

		void	reset();
		Handle	add(void* entry, uint32_t type);
		void	update(Handle& handle, void* entry);
		void	remove(const Handle& handle);

		template<typename T> T* get(Handle& handle) const;
	private:
		static const unsigned int MAX_ENTRIES = 65536;

		HandleEntry	entries[MAX_ENTRIES];
		int			activeEntryCounter;
		uint32_t	firstFreeEntry;
	};

	template<typename T>
	T* HandleManager::get(Handle& handle) const {
		const uint32_t index = handle.getIndex();
		if (entries[index].getCounter() == handle.getCounter() && entries[index].isActivated())
			return static_cast<T*>(entries[index].getEntry());
		return nullptr; // Handle is no longer valid
	}
}
#endif