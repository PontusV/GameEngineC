#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "Handle.h"
#include "scene/Scene.h"
#include <string>

namespace Core {
	/* An Entity handle with additional functionality. */
	class EntityHandle : public Handle {
	public:
		EntityHandle(Entity entity, Scene* scene);
		EntityHandle(const Handle& handle);
		EntityHandle();
		~EntityHandle();

		/* Adds the component at the end of the frame. Returns a temporary pointer to the component (it will become invalid after the end of the frame)*/
		template<typename T, class... Args>
		T* addComponent(Args&&... args);
		/* Adds the component at the end of the frame. Returns a temporary pointer to the component (it will become invalid after the end of the frame)*/
		template<typename T>
		T* addComponent(T component);
		/* Removes the component of type T at the end of the frame. */
		template<typename T>
		void removeComponent();

		bool operator==(const EntityHandle& other) const;
		bool operator!=(const EntityHandle& other) const;
	};

	// --------------------------- Template Function Definitions --------------------------------
	template<typename T, class... Args>
	T* EntityHandle::addComponent(Args&&... args) {
		T component(args...);
		T* temp = scene->addComponentQueued(entity, component);
		return temp;
	}

	template<typename T>
	T* EntityHandle::addComponent(T component) {
		T* temp = scene->addComponentQueued(entity, component);
		return temp;
	}

	template<typename T>
	void EntityHandle::removeComponent() {
		scene->removeComponentQueued<T>(entity);
	}
}
#endif