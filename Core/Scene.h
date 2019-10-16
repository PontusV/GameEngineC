#ifndef SCENE_H
#define SCENE_H

#include "Serializable.h"
#include "EntityManager.h"
#include "Handle.h"
#include "Entity.h"
#include "ObjectData.h"
#include "GameObjectData.h"
#include "UIObjectData.h"
#include "FunctionCaller.h"
#include <string>
#include <vector>
#include <queue>

namespace Core {
	/* Contains a collection of entities. */
	class Scene : public Serializable {
	public:
		Scene(EntityManager* entityManager, ObjectType type);
		~Scene();

		const std::vector<Handle>& getAllEntities() const;
		std::vector<Handle> getRootEntities();

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

		/* Returns a Handle to the Entity. */
		Handle getEntityHandle(Entity entity);
		/* Finds the Entity by name and returns a Handle to the Entity. */
		Handle getEntityHandle(std::string name);

		EntityManager* getEntityManager();
		ObjectType getType();
		/* Calls all the functions in queue and clears it. */
		void processQueue();
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
		std::queue<IFunctionCaller*> functionQueue;

	private:
		const ObjectType type;
		std::vector<Handle> entities;		// A list of entities contained by this scene
		EntityManager* manager;				// Manages the Entities in this Scene
		bool isAwake;
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

	template<typename... Ts>
	Handle Scene::createEntityQueued(std::string name, Ts& ... components) {
		Entity entity = manager->generateEntity(name);
		std::vector<Component*> componentVec = { static_cast<Component*>(&components)... };
		for (Component* component : componentVec) {
			component->setOwner(Handle(entity, this));
		}
		functionQueue.push(new FunctionCaller<Handle, Scene, Entity, Ts&...>(&Scene::addEntity<Ts...>, *this, entity, components...));
		return Handle(entity, this);
	}

	template<typename T>
	T* Scene::addComponentQueued(Entity entity, T& component) {
		component.setOwner(Handle(entity, this));
		auto function = new FunctionCaller<void, Scene, Entity, T&>(&Scene::addComponent<T>, *this, entity, component);
		functionQueue.push(function);
		return function->getArgument<T>();
	}

	template<typename T>
	void Scene::removeComponentQueued(Entity entity) {
		functionQueue.push(new FunctionCaller<void, Scene, Entity>(&Scene::removeComponent<T>, *this, entity));
	}
}
#endif