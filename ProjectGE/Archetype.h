#ifndef ARCHETYPE_H
#define ARCHETYPE_H

//#include "EntityManager.h"
#include "Chunk.h"
#include "Component.h"
#include "ComponentType.h"
#include "ComponentArrayManager.h"
#include <vector>
#include <utility>
#include <tuple>
#include <type_traits>
#include <memory>
#include <stdexcept>

#include "QuickSort.h"
#include "TypeUtility.h"
using namespace GameEngine::util;

namespace GameEngine {

	/* Interface for Archetype<T, Ts...>. Abstract class. */
	class IArchetype {
	public:
		virtual ~IArchetype() {};

		virtual void addEntity(Entity entity, std::vector<Component*> components) = 0;
		virtual void removeEntity(Entity& entity) = 0;
		virtual bool isEmpty() = 0;

		virtual Component* getComponent(Entity entity, ComponentTypeID componentTypeID) = 0;
		virtual std::vector<Component*> getData(Entity entity) = 0;

		virtual std::shared_ptr<IArchetype> addComponentType(const ComponentTypeID typeID) = 0;
		virtual std::shared_ptr<IArchetype> removeComponentType(const ComponentTypeID typeID) = 0;

		virtual std::vector<ComponentTypeID> getTypeIDs() = 0;
		//constexpr virtual int getTypeIDs() = 0; //C++ 20
	};

	/* Contains entities with the archetype specified by the template. If the types of components contained by an Entity matches the template it is added to the first chunk with room left. */
	template <typename... Ts>
	class Archetype : public IArchetype {
	public:
		Archetype() {
			static_assert(uniqueTypes<Ts...>::value, "Types in an Archetype must be unique!");
		}
		~Archetype() {
			while (!chunks.empty())
				removeChunk(0);
		}
		std::vector<ComponentTypeID> getTypeIDs() override {
			return { Ts::TYPE_ID... };
		}

		/* Components need to be sorted (by TypeID) */
		void addEntity(Entity entity, std::vector<Component*> components) override { //Override
			if (components.size() != sizeof...(Ts))
				throw std::invalid_argument("Invalid amount of Components given in Archetype!");

			std::size_t i = 0;
			addEntity(entity, static_cast<Ts*>(components[i++])...);
		}

		void addEntity(Entity entity, Ts*... components) {
			//Checking for space in existing chunks
			for (std::shared_ptr<Chunk<Ts...>> chunk : chunks) {
				if (!chunk->isFull()) {
					chunk->add(entity, components...);
					return;
				}
			}

			//No space found, adding new chunk
			createChunk();
			chunks.back()->add(entity, components...);
		}

		void removeEntity(Entity& entity) {
			for (std::size_t i = 0; i < chunks.size(); i++) {
				if (chunks[i]->contains(entity)) {
					chunks[i]->remove(entity);

					if (chunks[i]->isEmpty()) {
						removeChunk(i);
					}
					return;
				}
			}
		}

		Component* getComponent(Entity entity, ComponentTypeID componentTypeID) {
			for (std::shared_ptr<Chunk<Ts...>> chunk : chunks) {
				if (chunk->contains(entity)) {
					return chunk->getComponent(entity, componentTypeID);
				}
			}
			(std::cout << ... << Ts::TYPE_ID) << "\n";
			return nullptr;
		}

		bool isEmpty() {
			return chunks.empty();
		}


		std::vector<Component*> getData(Entity entity) {
			for (std::shared_ptr<Chunk<Ts...>> chunk : chunks) {
				if (chunk->contains(entity)) {
					std::tuple<Ts*...> dataRow = chunk->getData(entity);
					std::vector<Component*> components = std::apply([](auto&&... elems) {
						return std::vector<Component*>{elems...};
					}, dataRow);
					return components;
				}
			}
			throw std::invalid_argument("Entity was not found in this Archetype!");
		}
		//-----------------------------ADD-COMPONENT-------------------------------------------------------------------
		template <typename... Types>
		typename std::enable_if_t<uniqueTypes<Types...>::value, std::shared_ptr<IArchetype>> makeArchetype(TypeList<Types...>) {
			return std::make_shared< Archetype< Types... > >();
		}

		template<typename... Types>
		typename std::enable_if_t<!uniqueTypes<Types...>::value, std::shared_ptr<IArchetype>> makeArchetype(TypeList<Types...>) {
			throw std::invalid_argument("Could not add ComponentType that already exists in this Archetype!");
		}

		template <typename T, std::size_t... Is>
		std::shared_ptr<IArchetype> addComponentType(std::index_sequence<Is...>) { //Implementation
			constexpr auto sortedTypeIDs = sort(std::array{ T::TYPE_ID, Ts::TYPE_ID... });
			auto typeList = TypeList< typename decltype(getComponentType< sortedTypeIDs[Is] >())::type... >();

			return makeArchetype(typeList);
		}

		std::shared_ptr<IArchetype> addComponentType(const ComponentTypeID typeID, TypeList<>) {
			throw std::invalid_argument("Could not find ComponentType in TypeList when trying to add ComponentType.");
		}

		template <typename T, typename... Types>
		std::shared_ptr<IArchetype> addComponentType(const ComponentTypeID typeID, TypeList<T, Types...>) {
			if (typeID == T::TYPE_ID)
				return addComponentType<T>(std::make_index_sequence<sizeof...(Ts) + 1>{});

			return addComponentType(typeID, TypeList<Types...>());
		}

		std::shared_ptr<IArchetype> addComponentType(const ComponentTypeID typeID) override {
			TYPE_LIST typeList;
			return addComponentType(typeID, typeList);
		}
		//----------------------------------REMOVE-COMPONENT---------------------------------------------------------------------
		std::shared_ptr<IArchetype> removeComponentType(const ComponentTypeID typeID, TypeList<>) {
			throw std::invalid_argument("Could not find ComponentType in TypeList when trying to remove ComponentType.");
		}

		/* Creates and returns an archetype with the same types except the type specified in the template. */
		std::shared_ptr<IArchetype> removeComponentType(const ComponentTypeID typeID) override {
			TYPE_LIST typeList;
			return removeComponentType_impl(typeID, typeList);
		}

		template <typename T, typename... Types>
		std::shared_ptr<IArchetype> removeComponentType_impl(const ComponentTypeID typeID, TypeList<T, Types...>) {
			if (typeID == T::TYPE_ID) {
				if (!std::disjunction_v<std::is_same<T, Ts>...>)
					throw std::invalid_argument("Cannot remove a ComponentType that does not exist in this Archetype!");

				typename RemoveT<T, Ts...>::type typeList;
				return makeArchetype(typeList);
			}

			return addComponentType(typeID, TypeList<Types...>());
		}
		//----------------------------------------------------------------------------------------------------------------------
	private:

		void createChunk() {
			chunks.push_back(std::make_shared<Chunk<Ts...>>());
			//Notify listeners
			(ComponentArrayManager<Ts>::getInstance().chunkAdded(chunks.back().get()), ...);
		}

		void removeChunk(std::size_t index) {
			std::size_t chunkID = chunks[index]->getID();
			(ComponentArrayManager<Ts>::getInstance().chunkRemoved(chunkID), ...); //Notify listeners

			chunks.erase(chunks.begin() + index);
		}

		std::vector<std::shared_ptr<Chunk<Ts...>>> chunks;
	};
}
#endif