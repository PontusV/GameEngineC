#ifndef COMPONENT_HANDLE_H
#define COMPONENT_HANDLE_H

#include "ChunkEntityHandle.h"

namespace Core {

	class ComponentHandle {
	public:
		ComponentHandle(ChunkEntityHandle owner, ComponentTypeID typeID);
		ComponentHandle() {}
		~ComponentHandle();

		template <typename T> T* getComponent();
		Component*	getComponent();
		Entity		getOwner();
		bool		isValid();
	private:
		bool initialized;
		// Data to retrieve valid ptrs
		ChunkEntityHandle owner;
		ComponentTypeID typeID;

		// Ptrs
		Component* componentPtr;
		Entity* entityPtr;
	};

	template <typename T>
	T* ComponentHandle::getComponent() {
		return static_cast<T*>(getComponent());
	}
}
#endif