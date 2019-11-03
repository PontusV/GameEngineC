#ifndef SCENE_H
#define SCENE_H

#include "utils/Serializable.h"
#include "entity/EntityManager.h"
#include "entity/Entity.h"
#include "entity/handle/Handle.h"
#include "components/engine/ObjectData.h"
#include "components/engine/GameObjectData.h"
#include "components/engine/UIObjectData.h"
#include "utils/FunctionCaller.h"
#include <string>
#include <vector>
#include <deque>

namespace Core {
	/* Contains a collection of entities. */
	class Scene : public Serializable {
	public:
		Scene(EntityManager* entityManager, std::string name, ObjectType type);
		~Scene();

		const std::vector<Handle>& getAllEntities() const;
		std::vector<Handle> getRootEntities();
		std::string getName();

		/* Creates a new Entity in the Scene and adds it to the EntityManager. */
		template <typename... Ts>
		Handle createEntity(std::string name, Ts... components);
		template <typename... Ts>
		Handle addEntity(Entity entity, Ts&... components);
		/* Removes the Entity from the Scene and EntityManager. Returns true if successful */
		bool destroyEntity(Entity entity);
		template <typename T>
		/* Adds the component to the Entity. Awakes the component if the Scene is awake. */
		void addComponent(Entity entity, T& component);
		/* Removes the component from the Entity. */
		template<typename T>
		void removeComponent(Entity entity);
		void removeComponent(Entity entity, ComponentTypeID componentTypeID);

		template<typename... Ts>
		Handle createEntityQueued(std::string name, Ts& ... components);
		void destroyEntityQueued(Entity entity);
		/* Returns temporary pointer to the added component. This pointer will be invalid next frame. */
		template<typename T>
		T* addComponentQueued(Entity entity, T& component);
		template<typename T>
		void removeComponentQueued(Entity entity);
		void removeComponentQueued(Entity entity, ComponentTypeID componentTypeID);
		void setParentQueued(Handle entity, Handle parent);

		void setParent(Handle entity, Handle parent);
		Handle getParent(Entity entity) const;
		Handle getChild(Entity entity, int index) const;
		std::size_t getImmediateChildCount(Entity entity) const;

		void setSiblingIndex(Handle entity, std::size_t index);
		void setSiblingIndexQueued(Handle entity, std::size_t index);

		/* Activates the Entity and enables all behaviours. Activates all children. Cannot activate an Entity with an inactive parent. Returns true if successful. */
		bool activate(Handle entity);
		/* Deactivates the Entity and disables all behaviours. Deactivates all children. Returns true if successful. */
		bool deactivate(Handle entity);
		/* Activates the Entity and enables all behaviours next frame. Activates all children. Cannot activate an Entity with an inactive parent. */
		void activateQueued(Handle entity);
		/* Deactivates the Entity and disables all behaviours next frame. Deativates all children. */
		void deactivateQueued(Handle entity);

		/* Returns a Handle to the Entity. */
		Handle getEntityHandle(Entity entity);
		/* Finds the Entity by name and returns a Handle to the Entity. */
		Handle getEntityHandle(std::string name);

		EntityManager* getEntityManager();
		ObjectType getType();
		/* Calls all the functions in queue and clears it. */
		void processQueue();
		/* Retrieves component from queued createEntity and addComponent */
		template<typename T>
		T* getComponentFromQueue(Entity entity);
		/* Retrieves components from queued createEntity and addComponent */
		template<typename T>
		std::vector<T*> getComponentsFromQueue(Entity entity);

		/* Calls awake on all Entities contained by this Scene. Future Entities and Components will be awaken once added. Does nothing if the Scene has already been awaken. */
		void awake();
		/* Destroys all Entities contained by this Scene. */
		void clear();
		// Serializable
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

	private:
		void onEntityCreated(Entity entity); // Used by queue
		void onEntityCreated(Handle entity);
		void onEntityDestroyed(Entity entity, bool destroyingParent = false);
		void onEntityChanged(Handle entity);
		void awakeEntity(Handle entity);
		void awakeComponent(Handle entity, ComponentID componentID);
		bool destroyEntity(Entity entity, bool chained);

		void prepEntity(Handle entity);
		std::vector<Handle>::iterator findEntity(Entity entity);
		std::deque<IFunctionCaller*> functionQueue;
		std::map<Entity, std::vector<int>> entityQueueMap;

	private:
		const ObjectType type;
		std::vector<Handle> entities;		// A list of entities contained by this scene
		EntityManager* manager;				// Manages the Entities in this Scene
		bool isAwake;
		std::string name;
	};

	template <typename... Ts>
	Handle Scene::createEntity(std::string name, Ts... components) {
		Entity entity = manager->generateEntity(name);
		return addEntity(entity, components...);
	}

	template <typename... Ts>
	Handle Scene::addEntity(Entity entity, Ts&... components) {
		EntityLocation location;
		if (type == ObjectType::World) {
			GameObjectData component(this);
			location = manager->addEntity(entity, component, components...);
		}
		else {
			UIObjectData component(this);
			location = manager->addEntity(entity, component, components...);
		}

		Handle owner = Handle(entity, this);
		owner.updateLocation(location);
		prepEntity(owner);

		entities.push_back(owner);
		onEntityCreated(owner);

		return owner;
	}

	template <typename T>
	void Scene::addComponent(Entity entity, T& component) {
		EntityLocation location = manager->addComponent(entity, component);
		Handle handle = getEntityHandle(entity);
		handle.updateLocation(location);
		prepEntity(handle);
		if (isAwake) awakeComponent(getEntityHandle(entity), component.getComponentID());
		onEntityChanged(handle);
	}

	template<typename T>
	void Scene::removeComponent(Entity entity) {
		removeComponent(entity, typeIDof(T));
	}

	template<typename T>
	T* Scene::getComponentFromQueue(Entity entity) {
		auto it = entityQueueMap.find(entity);
		if (it == entityQueueMap.end()) return nullptr;

		for (const std::size_t& queueIndex : it->second) {
			std::vector<void*> argumentPtrs = functionQueue[queueIndex]->getArgumentPointers();
			for (std::size_t i = 1; i < argumentPtrs.size(); i++) {
				Component* component = static_cast<Component*>(argumentPtrs[i]);
				if (typeof(T) == component->getType().typeID) return static_cast<T*>(component);
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> Scene::getComponentsFromQueue(Entity entity) {
		std::vector<T*> components;
		auto it = entityQueueMap.find(entity);
		if (it == entityQueueMap.end()) return components;

		for (const std::size_t& queueIndex : it->second) {
			std::vector<void*> argumentPtrs = functionQueue[queueIndex]->getArgumentPointers();
			for (std::size_t i = 1; i < argumentPtrs.size(); i++) {
				Component* component = static_cast<Component*>(argumentPtrs[i]);
				if (typeof(T) == component->getType().typeID) components.push_back(static_cast<T*>(component));
			}
		}
		return components;
	}

	template<typename... Ts>
	Handle Scene::createEntityQueued(std::string name, Ts& ... components) {
		Entity entity = manager->generateEntity(name);
		std::vector<Component*> componentVec = { static_cast<Component*>(&components)... };
		for (Component* component : componentVec) {
			component->setOwner(Handle(entity, this));
		}

		// Add queue index to map
		entityQueueMap[entity].push_back(functionQueue.size());

		functionQueue.push_back(new FunctionCaller<Handle, Scene, Entity, Ts & ...>(&Scene::addEntity<Ts...>, *this, entity, components...));
		return Handle(entity, this);
	}

	template<typename T>
	T* Scene::addComponentQueued(Entity entity, T& component) {
		component.setOwner(Handle(entity, this));
		auto function = new FunctionCaller<void, Scene, Entity, T&>(&Scene::addComponent<T>, *this, entity, component);

		// Add queue index to map
		entityQueueMap[entity].push_back(functionQueue.size());

		functionQueue.push_back(function);
		return function->getArgument<T>();
	}

	template<typename T>
	void Scene::removeComponentQueued(Entity entity) {
		functionQueue.push_back(new FunctionCaller<void, Scene, Entity>(&Scene::removeComponent<T>, *this, entity));
	}
}
#endif