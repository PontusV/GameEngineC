#ifndef COMPONENT_ARRAY_MANAGER_H
#define COMPONENT_ARRAY_MANAGER_H

#include "Chunk.h"
#include "ComponentArray.h"
#include <vector>
#include <initializer_list>
#include <memory>

namespace Core {
	/* Notifies all ComponentArrays of creation and deletion of Chunks */
	class ComponentArrayManager {
	public:
		static ComponentArrayManager& getInstance() {
			static ComponentArrayManager instance;
			return instance;
		}
		ComponentArrayManager() {}
		~ComponentArrayManager() {
			for (IComponentArray* cmpArray : arrays)
				delete cmpArray;
		}

		template<typename T>
		ComponentArray<T>* createComponentArray(std::initializer_list<ComponentType> required = {}, std::initializer_list<ComponentType> ignoreables = {}) {
			ComponentArray<T>* cmpArray = new ComponentArray<T>(required, ignoreables);
			arrays.push_back(cmpArray);
			return cmpArray;
		}

		void chunkAdded(std::shared_ptr<Chunk> chunk, std::vector<ComponentTypeID> chunkTypes) {
			for (IComponentArray* a : arrays) {
				a->chunkAdded(chunk, chunkTypes);
			}
		}

		void chunkRemoved(std::size_t chunkID) {
			for (IComponentArray* a : arrays) {
				a->chunkRemoved(chunkID);
			}
		}

	private:
		std::vector<IComponentArray*> arrays;
	};
}
#endif