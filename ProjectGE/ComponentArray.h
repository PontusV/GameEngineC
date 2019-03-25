#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "Chunk.h"
#include "Component.h"
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <memory>

#include <iostream> //Test

namespace Core {
	struct ChunkInfo {
		ChunkInfo(void* dataPtr, std::shared_ptr<Chunk> chunkPtr) : dataPtr(dataPtr), chunkPtr(chunkPtr.get()) {}
		void* dataPtr;
		Chunk* chunkPtr;

		ChunkInfo& operator=(const ChunkInfo& other) {
			dataPtr = other.dataPtr;
			chunkPtr = other.chunkPtr;
			return *this;
		}
	};

	class IComponentArray {
	public:
		IComponentArray(ComponentTypeID typeID) : typeID(typeID) {}
		ComponentTypeID getTypeID() { return typeID; }

		virtual void chunkAdded(std::shared_ptr<Chunk> chunk, std::vector<ComponentTypeID> chunkTypes) = 0;
		virtual void chunkRemoved(std::size_t chunkID) = 0;

	private:
		ComponentTypeID typeID;
	};

	template <typename T>
	class ComponentArray : public IComponentArray {
	public:
		ComponentArray(std::initializer_list<ComponentTypeID> dependencies, std::initializer_list<ComponentTypeID> filter) : IComponentArray(T::TYPE_ID), dependecyTypeIDs(dependencies), filterTypeIDs(filter) {
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
			chunkMap.clear();
		}

		std::size_t size() {
			std::size_t sizeSum = 0;
			for (ChunkInfo& info : data) {
				sizeSum += info.chunkPtr->getSize();
			}
			return sizeSum;
		}

		T& get(std::size_t index) {
			std::size_t i = index;
			for (ChunkInfo& info : data) {
				std::size_t size = info.chunkPtr->getSize();
				if (i < size)
					return static_cast<T*>(info.dataPtr)[i];
				else
					i -= (size);

			}
			throw std::out_of_range("Index out of range.");
		}

		Entity& getEntity(std::size_t index) {
			std::size_t i = index;
			for (ChunkInfo& info : data) {
				std::size_t size = info.chunkPtr->getSize();
				if (i < size)
					return info.chunkPtr->getEntityArrayPtr()[i];
				else
					i -= (size);

			}
			throw std::out_of_range("Index out of range.");
		}

		T& operator[](std::size_t index) {
			return get(index);
		}

		void chunkAdded(std::shared_ptr<Chunk> chunk, std::vector<ComponentTypeID> chunkTypes) {
			if (isFiltered(chunkTypes)) return;

			chunkMap.insert(std::make_pair(chunk->getID(), data.size()));
			data.emplace_back(chunk->getComponentArrayPtr<T>(), chunk);
		}

		void chunkRemoved(std::size_t chunkID) {
			auto iterator = chunkMap.find(chunkID);
			if (iterator == chunkMap.end()) return;

			std::size_t index = (*iterator).second;

			if (index < data.size()-1) {
				std::iter_swap(data.begin() + index, data.end() - 1);
				chunkMap.at(data[index].chunkPtr->getID()) = index; //Remap swapped object
			}
			chunkMap.erase(iterator);
			data.pop_back();
		}

	private:
		bool isFiltered(std::vector<ComponentTypeID> typeIDs) {
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

		std::vector<ChunkInfo> data;
		std::map<std::size_t, std::size_t> chunkMap; //ID, index

		std::vector<ComponentTypeID> dependecyTypeIDs;
		std::vector<ComponentTypeID> filterTypeIDs;
	};
}

#endif