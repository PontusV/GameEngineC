#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "Chunk.h"
#include "Component.h"
#include "ComponentTypeInfo.h"
#include "ComponentType.h"

#include <vector>
#include <cstddef>
#include <stdexcept>
#include <map>
#include <memory>
#include <iostream>

namespace Core {
	struct ChunkDataArray {
		ChunkDataArray(ComponentDataArrayInfo info, std::shared_ptr<Chunk> chunkPtr) : beginPtr(info.beginPtr), chunkPtr(chunkPtr) {}
		char* beginPtr;
		std::shared_ptr<Chunk> chunkPtr;

		ChunkDataArray& operator=(const ChunkDataArray& other) {
			beginPtr = other.beginPtr;
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
		ComponentArray(std::initializer_list<ComponentType> dependencies, std::initializer_list<ComponentType> filter) : IComponentArray(typeof(T)), dependencyTypes(dependencies), filterTypes(filter) {
			for (const ComponentType& fType : filter) {
				if (type == fType) {
					std::cout << "You can not make a ComponentArray<T> filter the ComponentTypeID from T. " << type.getTypeID() << " : " << fType.getTypeID() << std::endl;
					throw std::invalid_argument("You can not make a ComponentArray<T> filter the ComponentTypeID from T.");
				}
				for (const ComponentType& dType : dependencies) {
					if (dType == fType) {
						std::cout << "You can not both filter and be dependent on a ComponentType in ComponentArray<T>. " << dType.getTypeID() << " : " << fType.getTypeID() << std::endl;
						throw std::invalid_argument("You can not both filter and be dependent on a ComponentType in ComponentArray<T>.");
					}
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
			for (ChunkDataArray& info : data) {
				std::size_t chunkSize = info.chunkPtr->getSize();
				if (index < chunkSize) {
					return *(T*)&info.beginPtr[info.chunkPtr->getStride() * index];
				}
				else {
					index -= chunkSize;
				}

			}
			std::cout << "ComponentArray::get::ERROR Index out of range. size: " << size() << ", typeID: " << T::getTypeID() << std::endl;
			throw std::out_of_range("ComponentArray::get::ERROR Index out of range.");
		}

		Entity& getEntity(std::size_t index) {
			for (ChunkDataArray& info : data) {
				std::size_t chunkSize = info.chunkPtr->getSize();
				if (index < chunkSize)
					return info.chunkPtr->getEntity(index);
				else
					index -= chunkSize;

			}
			throw std::out_of_range("ComponentArray::getEntity::ERROR Index out of range.");
		}

		T& operator[](std::size_t index) {
			return get(index);
		}

		void chunkAdded(std::shared_ptr<Chunk> chunk, std::vector<ComponentTypeID> chunkTypes) {
			if (filter(chunkTypes)) return;

			std::vector<ComponentDataArrayInfo> infoVec = chunk->getComponentArrayInfo(typeof(T));
			std::vector<std::size_t> indices;
			for (ComponentDataArrayInfo& info : infoVec) { // Add all matches
				ChunkDataArray chunkData(info, chunk);
				std::size_t repeat = calculateRepeat(chunkTypes);
				for (std::size_t i = 0; i < repeat; i++) {
					indices.push_back(data.size());
					data.push_back(chunkData);
				}
			}
			chunkMap.insert(std::make_pair(chunk->getID(), indices));
		}

		void chunkRemoved(std::size_t chunkID) {
			auto iterator = chunkMap.find(chunkID);
			if (iterator == chunkMap.end()) return;

			// Iterate through indices from chunkMap
			for (std::size_t& index : (*iterator).second) {
				if (index < data.size() - 1) {
					std::iter_swap(data.begin() + index, data.end() - 1);
					data[index].chunkPtr->getID();
					//std::cout << data.size() << ", " << index << ", " << data[index].chunkPtr.use_count() << std::endl;
					std::vector<std::size_t>& otherIndices = chunkMap.at(data[index].chunkPtr->getID());
					for (std::size_t& otherIndex : otherIndices) {
						if (otherIndex == data.size() - 1) {
							otherIndex = index;
							break;
						}
					}
				}
				data.pop_back();
			}
			chunkMap.erase(iterator);
		}

	private:
		bool filter(std::vector<ComponentTypeID> typeIDs) {
			if (typeIDs.empty()) return true;
			// Check FilterTypes
			for (const ComponentTypeID& typeID : typeIDs) {
				for (ComponentType& filterType : filterTypes) {
					if (filterType == typeID)
						return true; //Filter
				}
			}
			// Check DependencyTypes
			for (ComponentType& dependencyType : dependencyTypes) {
				for (std::size_t i = 0; i < typeIDs.size(); i++) {
					if (dependencyType == typeIDs[i]) { // Match
						break;
					}
					else if (i == typeIDs.size() - 1) { // Miss
						return true;
					}
				}
			}
			return false;
		}

		std::size_t calculateRepeat(std::vector<ComponentTypeID> typeIDs) {
			std::size_t repeat = 1;
			for (const ComponentType& dependencyType : dependencyTypes) {
				if (typeof(T) == dependencyType) break; // Skip self
				std::size_t count = 0;
				for (std::size_t i = 0; i < typeIDs.size(); i++) {
					if (dependencyType == typeIDs[i]) {
						count++;
					}
				}
				repeat *= count;
			}
			return repeat;
		}

		std::vector<ChunkDataArray> data;
		std::map<std::size_t, std::vector<std::size_t>> chunkMap; //ID, std::vector<indices>

		std::vector<ComponentType> dependencyTypes;
		std::vector<ComponentType> filterTypes;
	};
}

#endif