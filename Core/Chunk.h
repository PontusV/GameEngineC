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
		ComponentDataArrayInfo(char* beginPtr, ComponentTypeID typeID, std::size_t size) : size(size), typeID(typeID), beginPtr(beginPtr) {}
		ComponentTypeID typeID;
		std::size_t size;
		char* beginPtr;
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
		template<typename... Ts>
		void add(Entity entity, Ts&... componentPack);
		void destroy(Entity entity);
		void remove(Entity entity);

		template<typename T>
		void setComponent(Entity entity, T& component);
		template<typename T>
		void setComponent(std::size_t index, T& component);
		/* Returns first match. */
		template<typename T>
		T* getComponent(Entity entity);
		/* Returns first match. */
		Component* getComponent(Entity entity, ComponentType componentType);
		Component* getComponent(std::size_t index, ComponentType componentType);
		Component* getComponent(std::size_t index, ComponentTypeID componentTypeID);
		/* Returns all components attached to entity, who match with template T */
		template<typename T>
		std::vector<Component*> getComponents(Entity entity);
		/* Returns all components attached to entity, who match with componentType. */
		std::vector<Component*> getComponents(Entity entity, ComponentType componentType);
		/* Returns all components attached to entity at index, who match with componentType. */
		std::vector<Component*> getComponents(std::size_t index, ComponentType componentType);
		/* Returns all components attached to entity. */
		std::vector<Component*> getComponents(Entity entity);
		Entity* getEntityPtr(Entity entity);
		Entity& getEntity(std::size_t index);
		/* Returns start of component array containing the exact type from template. */
		template <typename T>
		char* getComponentBeginPtr();
		/* Returns info about all ComponentDataArrays matching the ComponentType */
		std::vector<ComponentDataArrayInfo> getComponentArrayInfo(ComponentType type);

		std::vector<ComponentDataBlock> getComponentDataBlocks(Entity entity);
		void copyEntity(Entity entity, std::vector<ComponentDataBlock> sources);

		std::size_t getIndex(Entity entity);
		std::vector<Component*> getComponents(std::size_t index);

		bool contains(Entity entity);
		bool isFull();
		bool isEmpty();

		std::size_t getID();
		std::size_t	getSize();
		std::size_t getStride();

	private:
		void destroy(std::size_t index);
		void remove(std::size_t index);
		char* getComponentBeginPtr(ComponentTypeID typeID);
		Component* getComponent(std::size_t index, ComponentDataArrayInfo& type);

		/* Returns start of Entity array. */
		char* getEntityBeginPtr();
		Entity* getEntityPtr(std::size_t index);

	private:
		std::size_t MAX_SIZE; // Size of entries
		std::size_t BUFFER_SIZE;
		char* buffer;
		std::size_t stride;

		std::size_t id;
		std::size_t size;
		std::vector<ComponentDataArrayInfo> types;
	};


	// --------------------------- Template Function Definitions --------------------------------

	template<typename... Ts>
	void Chunk::add(Entity entity, Ts&... componentPack) {
		if (isFull()) throw std::invalid_argument("Chunk::add::ERROR there is no more room in this chunk!");

		getEntity(size) = entity;
		(setComponent(size, componentPack), ...);
		size++;
	}

	template<typename T>
	void Chunk::setComponent(Entity entity, T& component) {
		std::size_t index = getIndex(entity);
		setComponent(index, component);
	}
	
	template<typename T>
	void Chunk::setComponent(std::size_t index, T& component) {
		char* componentBeginPtr = getComponentBeginPtr<T>();
		if (componentBeginPtr) {
			void* dest = &componentBeginPtr[index * stride];
			// Copy-constructor to copy the given component into memory
			T* newComponent = new(dest) T(component);
		}
		else {
			throw std::invalid_argument("Chunk::setComponent invalid component type! It does not exist in this chunk.");
		}
	}

	template<typename T>
	T* Chunk::getComponent(Entity entity) {
		return static_cast<T*>(getComponent(getIndex(entity), typeof(T)));
	}

	template<typename T>
	std::vector<Component*>	Chunk::getComponents(Entity entity) {
		return getComponents(entity, typeof(T));
	}

	template <typename T>
	char* Chunk::getComponentBeginPtr() {
		return getComponentBeginPtr(typeIDof(T));
	}
}
#endif