#ifndef COMPONENT_MANAGER_H2
#define COMPONENT_MANAGER_H2

#include <vector>
#include <map>
#include "Entity.h"

class HandleManager;
//template <typename T> class ComponentListener;

/* Management of a single component type */
template <typename T>
class ComponentManager2 {
public:
	static ComponentManager2<T>& getInstance() {
		static ComponentManager2<T> instance;
		return instance;
	}
	ComponentManager2(ComponentManager2 const&) = delete;
	void operator=(ComponentManager2 const&) = delete;
	ComponentManager2();
	~ComponentManager2();

	void add(Entity& entity, std::shared_ptr<T> component, HandleManager& handleManager);
	void remove(Entity& entity, HandleManager& handleManager);
	void update(Entity& entity, T& component, HandleManager& handleManager);
	T& get(Entity& entity);
private:
	std::vector<T> componentData; //Collection of unclaimed components. If there are no listeners for T the components will be stored here before they get fetched.
	std::map<Entity, int> entityMap;
};

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------

/* Contains entities with the archetype specified by the template. If the types of components contained by an Entity matches the the template it is added to the first chunk with room left. */
template <typename Ts...>
class Archetype {
	//Contains zero or more chunks
};

#include <vector>

template <std::size_t SIZE, typename T>
class ComponentDataArray {
public:
	ComponentDataArray() {
		data.reserve(SIZE);
	}
	void add(T component) {
		if (data.size() < SIZE)
			data.push_back(component);
	}
	void remove(int index) {
		data[index] = std::move(data.back);
		data.pop_back();
	}
	std::size_t size() {
		return data.size();
	}
	T& get(int index) {
		return data[index];
	}


private:
	std::vector<T> data;
};

#include <tuple>

/*  */
template <typename T, typename Ts...>
class Chunk {
public:
	static const std::size_t CHUNK_SIZE = 16000; //Size in bytes
	const std::size_t MAX_LENGTH = CHUNK_SIZE / (sizeof(T) + sizeof(Ts)...); //Number of elements (of each type)

	Chunk() {

	}

	void add(Entity entity, T componentData, T... componentDataPack) {

	}
private:
	std::size_t length;
private:
	std::tuple<ComponentDataArray<MAX_LENGTH, T>, ComponentDataArray<MAX_LENGTH, Ts>...> data;
};
#endif