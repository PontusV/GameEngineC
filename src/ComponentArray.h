#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <array>
#include <memory>
#include <map>
#include "Chunk.h"
#include "Component.h"

namespace GameEngine {
	template <typename T>
	struct ChunkArrayInfo {
		ChunkArrayInfo(T* data, std::shared_ptr<IChunk> chunkPtr) : data(data), chunkPtr(chunkPtr) {}
		T* data;
		std::shared_ptr<IChunk> chunkPtr;

		ChunkArrayInfo<T>& operator=(const ChunkArrayInfo<T>& other) {
			data = other.data;
			chunkPtr = other.chunkPtr;
			return *this;
		}
	};

	template <typename T>
	class ComponentArray {
	public:
		ComponentArray(std::initializer_list<ComponentTypeID> dependencies, std::initializer_list<ComponentTypeID> filter) : dependecyTypeIDs(dependencies), filterTypeIDs(filter) {
			for (const ComponentTypeID& fTypeID : filter) {
				if (T::TYPE_ID == fTypeID)
					throw std::invalid_argument("You can not make a ComponentArray<T> filter the ComponentTypeID from T.");
				for (const ComponentTypeID& dTypeID : dependencies) {
					if (dTypeID == fTypeID)
						throw std::invalid_argument("You can not both filter and be dependent on a ComponentType in ComponentArray<T>.");
				}
			}
		}
		~ComponentArray() {
			data.clear();
		}

		std::size_t size() {
			std::size_t sizeSum = 0;
			for (ChunkArrayInfo<T>& info : data) {
				sizeSum += info.chunkPtr->getSize();
			}
			return sizeSum;
		}

		T& get(std::size_t index) {
			std::size_t i = index;
			for (ChunkArrayInfo<T>& info : data) {
				std::size_t size = info.chunkPtr->getSize();
				if (i < size)
					return info.data[i];
				else
					i -= (size);

			}
			throw std::out_of_range("Index out of range.");
		}

		T& operator[](std::size_t index) {
			return get(index);
		}

		template <typename... Ts>
		void chunkAdded(std::shared_ptr<Chunk<Ts...>> chunk) {
			if (isFiltered({ Ts::TYPE_ID... })) return;
			//Map ID to ChunkArrayInfo index
			chunkMap.insert(std::make_pair(chunk->getID(), data.size()));
			data.emplace_back(chunk->getArrayPtr<T>(), chunk);

			std::cout << "ComponentArray<" << typeid(T).name() << ">\tChunk was added: ID = " << chunk->getID() << "\n";
		}

		void chunkRemoved(std::size_t chunkID) {
			auto iterator = chunkMap.find(chunkID);
			if (iterator == chunkMap.end()) return;

			std::size_t index = (*iterator).second;
			data.erase(data.begin() + index);

			for (std::size_t i = index; i < data.size(); i++) {
				chunkMap.at(data[i].chunkPtr->getID()) = i; //Remap
			}

			/*std::size_t index = (*iterator).second;
			std::iter_swap(data.begin() + index, data.end() - 1);
			data.pop_back();
			chunkMap[data[index].chunkID] = index; //Remap swapped object*/

			std::cout << "ComponentArray<" << typeid(T).name() << ">\tChunk was removed: ID = " << chunkID << " : " << data.size() << "\n";
		}

	private:
		bool isFiltered(std::initializer_list<ComponentTypeID> typeIDs) {
			std::size_t count = 0;
			for (const ComponentTypeID& typeID : typeIDs) {
				for (ComponentTypeID& filterID : filterTypeIDs) {
					if (typeID == filterID)
						return true; //Filter
				}
				for (ComponentTypeID& dependencyID : dependecyTypeIDs) {
					if (dependencyID == typeID) {
						count++;
						break;
					}
				}
			}
			if (dependecyTypeIDs.size() != count) return true;
			return false;
		}

		std::vector<ChunkArrayInfo<T>> data;
		std::vector<ComponentTypeID> dependecyTypeIDs;
		std::vector<ComponentTypeID> filterTypeIDs;
		std::map <std::size_t, std::size_t> chunkMap; //ID, index
	};
}

#endif