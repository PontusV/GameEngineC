#ifndef CHUNK_H
#define CHUNK_H

#include "Entity.h"
#include "ComponentTypeInfo.h"
#include "ComponentType.h"
#include <vector>

namespace Core {

	class Component; // Forward declare

	struct ComponentDataBlock {
		void* ptr;
		ComponentTypeID typeID;
	};

	struct ComponentDataArrayInfo {
		ComponentDataArrayInfo(ComponentTypeID typeID, std::size_t sizePerEntry) : sizePerEntry(sizePerEntry), typeID(typeID) {}
		ComponentTypeID typeID;
		std::size_t sizePerEntry;
		char* ptr;
	};

	class ChunkIDCounter {
	protected:
		static int idCounter;
	};

	class Chunk : private ChunkIDCounter {
	public:
		static constexpr const std::size_t CHUNK_SIZE = 64000; // Size in bytes

		Chunk(std::vector<IComponentTypeInfo> types);
		~Chunk();

		/* Adds entity and components to the back of the chunk. */
		template<typename... Ts> void					add(Entity entity, Ts&... componentPack);
		void											remove(Entity entity, bool destruct = true);

		template<typename T> void						setComponent(Entity entity, T& component);
		template<typename T> void						setComponent(std::size_t index, T& component);
		/* Returns first match. */
		template<typename T> T*							getComponent(Entity entity);
		/* Returns first match. */
		Component*										getComponent(Entity entity, ComponentType componentType);
		Component*										getComponent(std::size_t index, ComponentType componentType);
		Component*										getComponent(std::size_t index, ComponentTypeID componentTypeID);
		/* Returns all components attached to entity, who match with template T */
		template<typename T> std::vector<Component*>	getComponents(Entity entity);
		/* Returns all components attached to entity, who match with componentType. */
		std::vector<Component*>							getComponents(Entity entity, ComponentType componentType);
		/* Returns all components attached to entity at index, who match with componentType. */
		std::vector<Component*>							getComponents(std::size_t index, ComponentType componentType);
		/* Returns all components attached to entity. */
		std::vector<Component*>							getComponents(Entity entity);
		std::vector<ComponentDataBlock>					getComponentDataBlocks(Entity entity);

		Entity*											getEntityPtr(Entity entity);
		/* Returns start of Entity array. */
		Entity*											getEntityArrayPtr();
		/* Returns start of component array containing the exact type from template. */
		template <typename T> T*						getComponentArrayPtr();
		/* Returns vector of pointers to the start of all component array containing components types deriving from the type in the template. */
		template <typename T> std::vector<T*>			getComponentArrayPtrs();
		/* Returns info about all ComponentDataArrays matching the ComponentType */
		std::vector<ComponentDataArrayInfo>				getComponentArrayInfo(ComponentType type);

		void											copyEntity(Entity entity, std::vector<ComponentDataBlock> sources);

		bool											contains(Entity entity);
		bool											isFull();
		bool											isEmpty();

		std::size_t										getID();
		std::size_t										getSize();

		std::size_t										getIndex(Entity entity);
		std::vector<Component*>							getComponents(std::size_t index);

	private:
		void											remove(std::size_t index, bool destroy = true);
		char*											getComponentArrayPtr(ComponentTypeID typeID);
		/* Returns all ComponentDataArrays matching the given type. */
		std::vector<char*>								getComponentArrayPtrs(ComponentType type);
		Component*										getComponent(std::size_t index, ComponentDataArrayInfo& type);

	private:
		std::size_t MAX_SIZE; // Size of entries
		std::size_t BUFFER_SIZE;
		char* buffer;

		std::size_t id;
		std::size_t size;
		std::vector<ComponentDataArrayInfo> types;
	};


	// --------------------------- Template Function Definitions --------------------------------

	template<typename... Ts>
	void Chunk::add(Entity entity, Ts&... componentPack) {
		if (isFull()) throw std::invalid_argument("Chunk::add::ERROR there is no more room in this chunk!");

		getEntityArrayPtr()[size] = entity;
		(setComponent(size, componentPack), ...);
		size++;
	}

	template<typename T>
	void Chunk::setComponent(Entity entity, T& component) {
		std::size_t index = getIndex(entity);
		setComponent(index, component);
	}
	
	/* ComponentArrayPtr has to be for type T, and not any of its base classes. */
	template<typename T>
	void Chunk::setComponent(std::size_t index, T& component) {
		T* componentArrayPtr = getComponentArrayPtr<T>();
		T* componentPtr = &componentArrayPtr[index];

		// Force copy-constructor to copy the given component into memory
		T* newComponent = new(componentPtr) T(component);
	}

	template<typename T>
	T* Chunk::getComponent(Entity entity) {
		return static_cast<T*>(getComponent(getIndex(entity), typeof(T)));
	}

	template<typename T>
	std::vector<T*>	getComponents(Entity entity) {
		return getComponents(entity, typeof(T));
	}

	template <typename T>
	T* Chunk::getComponentArrayPtr() {
		return (T*)getComponentArrayPtr(typeIDof(T));
	}

	template <typename T>
	std::vector<T*> Chunk::getComponentArrayPtrs() {
		std::vector<T*> arrayVec;
		std::vector<char*> rawArrayVec = getComponentArrayPtrs(typeof(T));
		// Convert ptr to T*
		arrayVec.reserve(rawArrayVec.size());
		for (char* ptr : rawArrayVec) {
			arrayVec.push_back(ptr);
		}
		return arrayVec;
	}
}
#endif