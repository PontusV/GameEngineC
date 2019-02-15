#ifndef COMPONENT_ARRAY_MANAGER_H
#define COMPONENT_ARRAY_MANAGER_H

#include "Chunk.h"
#include <vector>
#include "ComponentArray.h"
#include <initializer_list>

namespace GameEngine {
	/* Notifies all ComponentArrays of creation and deletion of Chunks */
	template <typename T>
	class ComponentArrayManager {
	public:
		static ComponentArrayManager<T>& getInstance() {
			static ComponentArrayManager instance;
			return instance;
		}
		ComponentArrayManager() {}
		~ComponentArrayManager() {
			arrays.clear();
		}

		ComponentArray<T>& createComponentArray() {
			return createComponentArray({}, {});
		}
		ComponentArray<T>& createComponentArray(std::initializer_list<ComponentTypeID> required) {
			return createComponentArray(required, {});
		}
		ComponentArray<T>& createComponentArray(std::initializer_list<ComponentTypeID> required, std::initializer_list<ComponentTypeID> ignoreables) {
			std::cout << "ComponentArray created!\n";
			arrays.emplace_back(required, ignoreables);
			return arrays.back();
		}
		//--
		template <typename... Ts>
		void chunkAdded(std::shared_ptr<Chunk<Ts...>> chunk) {
			//std::cout << "ComponentArrayManager<" << typeid(T).name() << ">\tChunk was added: ID = " << chunk.getID() << "\n";
			for (ComponentArray<T>& a : arrays) {
				a.chunkAdded(chunk);
			}
		}

		void chunkRemoved(std::size_t chunkID) {
			//std::cout << "ComponentArrayManager<" << typeid(T).name() << ">\tChunk was removed: ID = " << chunkID << "\n";
			for (ComponentArray<T>& a : arrays) {
				a.chunkRemoved(chunkID);
			}
		}
		//--
	private:

		std::vector<ComponentArray<T>> arrays;
	};
}
#endif