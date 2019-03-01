#ifndef COMPONENT_ARRAY_MANAGER_H
#define COMPONENT_ARRAY_MANAGER_H

#include "Chunk.h"
#include "ComponentArray.h"
#include <vector>
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
		}

		ComponentArray<T>& createComponentArray() {
			return createComponentArray({}, {});
		}
		ComponentArray<T>& createComponentArray(std::initializer_list<ComponentTypeID> required) {
			return createComponentArray(required, {});
		}
		ComponentArray<T>& createComponentArray(std::initializer_list<ComponentTypeID> required, std::initializer_list<ComponentTypeID> ignoreables) {
			arrays.emplace_back(std::make_shared<ComponentArray<T>>(required, ignoreables));
			return *arrays.back();
		}
		//--
		template <typename... Ts>
		void chunkAdded(Chunk<Ts...>* chunk) {
			//std::cout << "ComponentArrayManager<" << typeid(T).name() << ">\tChunk was added: ID = " << chunk.getID() << "\n";
			for (std::shared_ptr<ComponentArray<T>> a : arrays) {
				a->chunkAdded(chunk);
			}
		}

		void chunkRemoved(std::size_t chunkID) {
			//std::cout << "ComponentArrayManager<" << typeid(T).name() << ">\tChunk was removed: ID = " << chunkID << "\n";
			for (std::shared_ptr<ComponentArray<T>> a : arrays) {
				a->chunkRemoved(chunkID);
			}
		}
		//--
	private:

		std::vector< std::shared_ptr<ComponentArray<T>> > arrays; //Fix move in ComponentArray and switch from pointers to the real thing. ChunkPtr inside ComponentArray becomes invalid when ComponentArray is moved
	};
}
#endif