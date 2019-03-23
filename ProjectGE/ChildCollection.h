#ifndef CHILD_COLLECTION_H
#define CHILD_COLLECTION_H

#include "ChunkEntityHandle.h"
#include "Entity.h"
#include "ComponentHandle.h"
#include "ChildListener.h"
#include <vector>

namespace Core {

	class Component; // Forward declare

	template <typename T>
	class ChildCollection : public ChildListener {
	public:
		ChildCollection() {}
		~ChildCollection() {}

		void childAdded(ChunkEntityHandle entity);
		void childRemoved(Entity entity);

		std::vector<T*> getCollection();
		void clear();
	private:
		std::vector<ComponentHandle> collection;
	};

	// ---------------------Function Definitions--------------------------------
	template <typename T>
	void ChildCollection<T>::childAdded(ChunkEntityHandle entity) {
		try {
			std::vector<Component*> components = entity.getComponents();
			for (Component* component : components) {
				if (dynamic_cast<T*>(component)) {
					collection.emplace_back(entity, component->getTypeID());
				}
			}
		}
		catch (std::exception& e) {
			std::cout << "ChildCollection::childAdded::error There was an error when trying to add child.\n";
			std::cout << e.what() << "\n";
		}
	}

	template <typename T>
	void ChildCollection<T>::childRemoved(Entity entity) {
		for (auto it = collection.begin(); it != collection.end();) {
			if (it->getOwner() == entity)
				it = collection.erase(it);
			else
				it++;
		}
	}

	template <typename T>
	std::vector<T*> ChildCollection<T>::getCollection() {
		std::vector<T*> children;
		children.reserve(collection.size());

		for (ComponentHandle& handle : collection) {
			children.push_back(handle.getComponent<T>());
		}

		return children;
	}

	template <typename T>
	void ChildCollection<T>::clear() {
		collection.clear();
	}
}
#endif