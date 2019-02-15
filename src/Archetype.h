#ifndef ARCHETYPE_H
#define ARCHETYPE_H

//#include "EntityManager.h"
#include "Chunk.h"
#include "ComponentType.h"
#include "ComponentArrayManager.h"
#include <vector>
#include <utility>
#include <tuple>
#include <type_traits>
#include <memory>
#include <stdexcept>

#include "QuickSort.h" //Test

namespace GameEngine {
	class Component;

	namespace {
		template <typename T, typename TList, typename Res = TypeList<>>
		struct removeT_helper;

		template<typename T, typename Res>
		struct removeT_helper<T, TypeList<>, Res> {
			using type = Res;
		};

		template<typename T, typename... Ts, typename... TRes>
		struct removeT_helper<T, TypeList<T, Ts...>, TypeList<TRes...>> :
			removeT_helper<T, TypeList<Ts...>, TypeList<TRes...>>
		{};

		template<typename T, typename T1, typename ...Ts, typename... TRes>
		struct removeT_helper<T, TypeList<T1, Ts...>, TypeList<TRes...>> :
			removeT_helper<T, TypeList<Ts...>, TypeList<TRes..., T1>>
		{};

		template <typename T, typename...Ts> struct RemoveT {
			using type = typename removeT_helper<T, TypeList<Ts...>>::type;
		};
		//----

		template <typename T, typename T2>
		struct is_not_same {
			static constexpr bool value = !std::is_same<T, T2>::value;
		};

		template <typename...> struct uniqueTypes;
		template <>
		struct uniqueTypes<> {
			static constexpr bool value = true;
		};
		template <typename T>
		struct uniqueTypes<T> {
			static constexpr bool value = true;
		};
		template <typename T, typename... Ts>
		struct uniqueTypes<T, Ts...> {
			static constexpr bool value = std::conjunction<uniqueTypes<Ts...>, is_not_same<T, Ts>...>::value;
		};
	}

	/* Interface for Archetype<T, Ts...>. Abstract class. */
	class IArchetype {
	public:
		//template<typename T>
		//virtual ComponentArray<T>& getComponentArray() = 0; //Might not be needed (Use inject into ComponentGroup instead?)
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

		//Test functions below
		//virtual void addComponent(Entity& entity, Component* component) = 0;
	};

	/* Contains entities with the archetype specified by the template. If the types of components contained by an Entity matches the template it is added to the first chunk with room left. */
	template <typename... Ts>
	class Archetype : public IArchetype {
	public:
		//Contains zero or more chunks
		Archetype() {
			static_assert(uniqueTypes<Ts...>::value, "Types in an Archetype must be unique!");
			//if (!uniqueTypes<Ts...>::value)
			//	throw std::invalid_argument("Cannot have multiple of same ComponentType in an Archetype!");
		}
		~Archetype() {
			chunks.clear();
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

	private:

		void createChunk() {
			chunks.push_back(std::make_shared<Chunk<Ts...>>());
			//Notify listeners
			(ComponentArrayManager<Ts>::getInstance().chunkAdded(chunks.back()), ...); //Notifies all component managers of this type
		}

		void removeChunk(std::size_t index) {
			std::size_t chunkID = chunks[index]->getID();
			chunks.erase(chunks.begin() + index);
			//Notify listeners
			(ComponentArrayManager<Ts>::getInstance().chunkRemoved(chunkID), ...);
		}

		std::vector<std::shared_ptr<Chunk<Ts...>>> chunks;
	};
}
#endif