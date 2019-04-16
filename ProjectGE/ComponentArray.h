#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "Chunk.h"
#include "Component.h"
#include "ComponentTypeInfo.h"
#include "ComponentType.h"

#include <vector>
#include <cstddef>
#include <stdexcept>
#include <algorithm> //?
#include <map>
#include <memory>

#include <iostream> //Test

namespace Core {
	struct ChunkDataArray {
		ChunkDataArray(ComponentDataArrayInfo info, std::shared_ptr<Chunk> chunkPtr) : dataPtr(info.ptr), sizePerEntry(info.sizePerEntry), chunkPtr(chunkPtr.get()) {}
		char* dataPtr;
		std::size_t sizePerEntry;
		Chunk* chunkPtr;

		ChunkDataArray& operator=(const ChunkDataArray& other) {
			dataPtr = other.dataPtr;
			sizePerEntry = other.sizePerEntry;
			chunkPtr = other.chunkPtr;
			return *this;
		}
	};

	class IComponentArray {
	public:
		IComponentArray(ComponentType type) : type(type) {}
		const ComponentType& getType() const { return type; }

		virtual void chunkAdded(std::shared_ptr<Chunk> chunk, std::vector<ComponentTypeID> chunkTypes) = 0;
		virtual void chunkRemoved(std::size_t chunkID) = 0;

	protected:
		ComponentType type;
	};

	template <typename T>
	class ComponentArray : public IComponentArray {
	public:
		ComponentArray(std::initializer_list<ComponentType> dependencies, std::initializer_list<ComponentType> filter) : IComponentArray(typeof(T)), dependecyTypes(dependencies), filterTypes(filter) {
			for (const ComponentType& fType : filter) {
				if (type == fType)
					throw std::invalid_argument("You can not make a ComponentArray<T> filter the ComponentTypeID from T.");
				for (const ComponentType& dType : dependencies) {
					if (dType == fType)
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
			for (ChunkDataArray& info : data) {
				sizeSum += info.chunkPtr->getSize();
			}
			return sizeSum;
		}

		T& get(std::size_t index) {
			std::size_t i = index;
			for (ChunkDataArray& info : data) {
				std::size_t size = info.chunkPtr->getSize();
				if (i < size) {
					return *((T*)(&(info.dataPtr[i*info.sizePerEntry])));
				}
				else {
					i -= (size);
				}

			}
			throw std::out_of_range("Index out of range.");
		}

		Entity& getEntity(std::size_t index) {
			std::size_t i = index;
			for (ChunkDataArray& info : data) {
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
			if (filter(chunkTypes)) return;

			chunkMap.insert(std::make_pair(chunk->getID(), data.size()));
			std::vector<ComponentDataArrayInfo> infoVec = chunk->getComponentArrayInfo(typeof(T));
			for (ComponentDataArrayInfo& info : infoVec) { // Add all matches
				data.push_back(ChunkDataArray(info, chunk));
			}
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
		bool filter(std::vector<ComponentTypeID> typeIDs) {
			std::size_t count = 0;
			for (const ComponentTypeID& typeID : typeIDs) {
				for (ComponentType& filterType : filterTypes) {
					if (filterType == typeID)
						return true; //Filter
				}
				for (ComponentType& dependencyType : dependecyTypes) {
					if (dependencyType == typeID) {
						count++;
						break;
					}
				}
			}
			if (dependecyTypes.size() != count) return true;
			return false;
		}

		std::vector<ChunkDataArray> data;
		std::map<std::size_t, std::size_t> chunkMap; //ID, index

		std::vector<ComponentType> dependecyTypes;
		std::vector<ComponentType> filterTypes;
	};
}

#endif