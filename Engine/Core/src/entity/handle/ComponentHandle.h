#ifndef COMPONENT_HANDLE_H
#define COMPONENT_HANDLE_H

#include "Handle.h"
#include "entity/component/IComponentData.h"

namespace Core {
	class EntityManager;

	/* A handle for a component with a specific ComponentTypeID. Does not look for components with a type deriving from the type with the specified ComponentTypeID. */
	template<typename T>
	class TComponentHandle {
	public:
		TComponentHandle(ComponentTypeID typeID, Entity entity, EntityManager* entityManager) : typeID(typeID), owner(Handle(entity, entityManager)) {}
		TComponentHandle(ComponentTypeID typeID, Handle owner) : typeID(typeID), owner(Handle(owner)) {}
		TComponentHandle() {}
		virtual ~TComponentHandle() {}

		bool operator==(const TComponentHandle<T>& other) const {
			return owner == other.owner && typeID == other.typeID;
		}
		bool operator!=(const TComponentHandle<T>& other) const {
			return !(*this == other);
		}

		T* get() {
			return static_cast<T*>(owner.getComponent(typeID));
		}
		bool isValid() {
			return owner.refresh() && owner.hasComponent(typeID);
		}
		void clear() {
			owner = Handle();
			typeID = 0;
		}

		void setOwner(Handle handle) {
			owner = handle;
		}
		void setComponentTypeID(ComponentTypeID typeID) {
			this->typeID = typeID;
		}
		Handle getOwner() const {
			return owner;
		}
		const Handle& getOwnerRef() const {
			return owner;
		}
		ComponentTypeID getComponentTypeID() const {
			return typeID;
		}

	private:
		Handle owner;
		ComponentTypeID typeID = 0;
	};

	/* A handle for a component with a specific ComponentTypeID. Does not look for components with a type deriving from the type with the specified ComponentTypeID. */
	class ComponentHandle : public TComponentHandle<IComponentData> {
	public:
		ComponentHandle(ComponentTypeID typeID, Entity entity, EntityManager* entityManager) : TComponentHandle<IComponentData>(typeID, entity, entityManager) {}
		ComponentHandle(ComponentTypeID typeID, Handle owner) : TComponentHandle<IComponentData>(typeID, owner) {}
		ComponentHandle() {}
		~ComponentHandle() {}

		bool operator==(const ComponentHandle& other) const {
			return TComponentHandle<IComponentData>::operator==(other);
		}
		bool operator!=(const ComponentHandle& other) const {
			return !(*this == other);
		}

		IComponentData* getComponent() {
			return TComponentHandle<IComponentData>::get();
		}

		template<typename T>
		T* getComponent() {
			return static_cast<T*>(TComponentHandle<IComponentData>::get());
		}
	};
}
#endif