#ifndef CHUNK_H
#define CHUNK_H

#include <utility>
#include <tuple>
#include <stdexcept>
#include <array>
#include "Entity.h"

namespace GameEngine {
	namespace { //Helpers. Required because (0 + ... + sizeof(Ts)) does not work with static constexpr; the problem is the fold expression.
		template <typename...>
		struct SizeOf;

		template <typename T>
		struct SizeOf<T> {
			static constexpr std::size_t value = (sizeof(T));
		};

		template <typename T, typename... Ts>
		struct SizeOf<T, Ts...> {
			static constexpr std::size_t value = (sizeof(T) + SizeOf<Ts...>::value);
		};
	}


	class IChunk {
	public:
		virtual std::size_t getID() = 0;
		virtual std::size_t getSize() = 0;
	};

	static std::size_t ChunkIDCounter = 0;

	/* "all components of type A, are tightly packed in an array. Followed by all components of type B etc."
	Contains an array for each type in template
	Contains an array of entityIDs
	Array size set for them to all fit in a set amount of memory (see: CHUNK_SIZE)
	*/
	template <typename... Ts>
	class Chunk : public IChunk {
	public:
		static constexpr const std::size_t CHUNK_SIZE = 16000; //Size in bytes
		static constexpr const std::size_t MAX_SIZE = CHUNK_SIZE / (sizeof(Entity) + SizeOf<Ts...>::value);

		Chunk() {
			size = 0;
			id = ChunkIDCounter++;
		}

		~Chunk() {}

		/* Adds entity and components to the back of the chunk. */
		void add(Entity entity, Ts*... componentPack) {
			add_impl(entity, componentPack..., std::make_index_sequence<1 + sizeof...(componentPack)>{});
		}

		/* Removes specified entity from chunk if found. */
		void remove(Entity& entity) {
			std::array<Entity, MAX_SIZE>& entities = std::get<0>(data);
			for (std::size_t i = 0; i < size; i++) {
				if (entities[i] == entity)
					remove(i);
			}
		}

		/* Looks to see if the given entity is contained in this chunk. */
		bool contains(Entity& entity) {
			std::array<Entity, MAX_SIZE>& entities = std::get<0>(data);
			for (std::size_t i = 0; i < size; i++) {
				if (entities[i] == entity)
					return true;
			}
			return false;
		}

		Component* getComponent(Entity entity, ComponentTypeID componentTypeID) {
			return getComponent<1, Ts...>(entity, componentTypeID);
		}

		/* Moves data row containing specified Entity into tuple and returns the tuple. Removes data row containing specified Entity. */
		std::tuple<Ts*...> getData(Entity& entity) {
			std::size_t index = getIndex(entity);
			return getDataAtIndex(index);
		}

		/* Returns a tuple containing copies of the data at index. */
		inline std::tuple<Ts*...> getDataAtIndex(std::size_t index) {
			if (index >= size)
				throw std::out_of_range("Cannot get data at index. Index out of bounds!");

			return getDataAtIndex_impl(index, std::make_index_sequence<1 + sizeof...(Ts)>{});
		}

		bool isFull() {
			return size >= MAX_SIZE;
		}

		bool isEmpty() {
			return size == 0;
		}

		std::size_t getID() {
			return id;
		}

		std::size_t getSize() {
			return size;
		}

		template <typename T>
		T* getArrayPtr() {
			return std::get<std::array<T, MAX_SIZE>>(data).data();
		}

	private:
		template <std::size_t... Is>
		void remove_impl(std::size_t index, std::index_sequence<Is...>) {
			size--;
			if (!isEmpty()) {
				(std::swap(std::get<Is>(data)[index], std::get<Is>(data)[size]), ...); //This essentially removes the element at index
			}
		}

		void remove(std::size_t index) {
			remove_impl(index, std::make_index_sequence<1 + sizeof...(Ts)>{});
		}

		template <std::size_t indexZero, std::size_t... Is>
		void add_impl(Entity entity, Ts*... componentPack, std::index_sequence<indexZero, Is...>) {
			std::get<0>(data)[size] = entity;
			(void(std::get<Is>(data)[size] = *componentPack), ...);
			size++;
		}

		template <std::size_t indexZero, std::size_t... Is>
		inline std::tuple<Ts*...> getDataAtIndex_impl(std::size_t index, std::index_sequence<indexZero, Is...>) {
			return std::make_tuple(&std::get<Is>(data)[index]...);
		}

		std::size_t getIndex(Entity& entity) {
			std::array<Entity, MAX_SIZE>& entities = std::get<0>(data);
			for (std::size_t i = 0; i < size; i++) {
				if (entities[i] == entity)
					return i;
			}
			throw std::invalid_argument("Entity is not contained in this chunk!");
		}

		template <std::size_t I = 1, typename T, typename... Ts>
		Component* getComponent(Entity entity, ComponentTypeID componentTypeID) {
			if (componentTypeID == T::TYPE_ID) {
				std::size_t index = getIndex(entity);
				return &std::get<I>(data)[index];
			}
			return getComponent<I + 1, Ts...>(entity, componentTypeID);
		}

		template <std::size_t I>
		Component* getComponent(Entity entity, ComponentTypeID componentTypeID) {
			throw std::invalid_argument("Entity does not have that Component.");
			return nullptr;
		}

		std::size_t size;
		std::tuple<std::array<Entity, MAX_SIZE>, std::array<Ts, MAX_SIZE>...> data; //tuple<Entity[], ComponentDataArray1, ComponentDataArray2...>
		std::size_t id;
	};
}
#endif