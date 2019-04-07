#ifndef CHUNK_H
#define CHUNK_H

#include "Entity.h"
#include "ComponentTypeInfo.h"
#include <vector>

namespace Core {

	typedef std::size_t ComponentTypeID;
	class Component; // Forward declare

	struct ComponentDataBlock {
		void* ptr;
		ComponentTypeID typeID;
	};

	class ChunkIDCounter {
	protected:
		static int idCounter;
	};

	class Chunk : private ChunkIDCounter {
	public:
		static constexpr const std::size_t CHUNK_SIZE = 64000; // Size in bytes

		Chunk(std::vector<ComponentTypeInfo> types);
		~Chunk();

		/* Adds entity and components to the back of the chunk. */
		template<typename... Ts> void		add(Entity entity, Ts*... componentPack);
		void								remove(Entity entity, bool destruct = true);

		template<typename T> void			setComponent(Entity entity, T* component);
		template<typename T> void			setComponent(std::size_t index, T* component);
		template<typename T> T*		getComponent(Entity entity);
		Component*							getComponent(Entity entity, ComponentTypeID componentTypeID);
		std::vector<Component*>				getComponents(Entity entity);
		std::vector<ComponentDataBlock>		getComponentDataBlocks(Entity entity);

		Entity*								getEntityPtr(Entity entity);
		Entity*								getEntityArrayPtr();		// Returns start of Entity array.
		template <typename T>				T* getComponentArrayPtr();	// Returns start of component array containing the type from template.

		void								copyEntity(Entity entity, std::vector<ComponentDataBlock> sources);

		bool								contains(Entity entity);
		bool								isFull();
		bool								isEmpty();

		std::size_t							getID();
		std::size_t							getSize();

		std::size_t							getIndex(Entity entity);
		std::vector<Component*>				getComponents(std::size_t index);

	private:
		void								remove(std::size_t index, bool destroy = true);
		char*								getComponentArrayPtr(ComponentTypeID typeID);
		Component*							getComponent(std::size_t index, ComponentTypeID componentTypeID);
		Component*							getComponent(std::size_t index, ComponentTypeInfo& type);

	private:
		std::size_t MAX_SIZE; // Size of entries
		std::size_t BUFFER_SIZE;
		char* buffer;

		std::size_t id;
		std::size_t size;
		std::vector<ComponentTypeInfo> types;
	};


	// --------------------------- Template Function Definitions --------------------------------

	template<typename... Ts>
	void Chunk::add(Entity entity, Ts*... componentPack) {
		if (isFull()) throw std::invalid_argument("Chunk::add::ERROR there is no more room in this chunk!");

		getEntityArrayPtr()[size] = entity;
		(setComponent(size, componentPack), ...);
		size++;
	}

	template<typename T>
	void Chunk::setComponent(Entity entity, T* component) {
		std::size_t index = getIndex(entity);
		setComponent(index, component);
	}

	template<typename T>
	void Chunk::setComponent(std::size_t index, T* component) {
		T* componentArrayPtr = getComponentArrayPtr<T>();
		T* componentPtr = &componentArrayPtr[index];

		// Force copy-constructor to copy the given component into memory
		T* newComponent = new(componentPtr) T();
		*newComponent = *component;
	}

	template<typename T>
	T* Chunk::getComponent(Entity entity) {
		ComponentTypeInfo type(sizeof(T), T::TYPE_ID);
		return static_cast<T*>(getComponent(getIndex(entity), type));
	}

	template <typename T>
	T* Chunk::getComponentArrayPtr() {
		return (T*)getComponentArrayPtr(T::TYPE_ID);
	}
}
#endif