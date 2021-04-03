#pragma once
#include "dllexport.h"
#include "Entity.h"
#include "HideFlags.h"

namespace Core {
	typedef std::size_t ComponentTypeID;
	class IScene;
	class IComponent;

	class DLLEXPORT IEntityHandle {
	public:
		int compare(IEntityHandle* other) {
			return getEntity().getID() - other->getEntity().getID();
		}
		/* Deletes this instance */
		virtual void release() = 0;
		/* Returns true if the given Entity is a child of the Entity this instance points to. */
		virtual bool isChild(Entity entity) = 0;
		/* Returns true if the given Entity is a parent of the Entity this instance points to. */
		virtual bool isParent(Entity entity) = 0;
		/* Returns true if the given Entity is an immediate child of the Entity this instance points to. */
		virtual bool isImmediateChild(Entity entity) = 0;

		/* Activates the Entity and enables all behaviours. */
		virtual void activate() = 0;
		/* Deactivates the Entity and disables all behaviours. */
		virtual void deactivate() = 0;
		virtual bool isActive() = 0;

		virtual const Entity& getEntity() const = 0;
		virtual IScene* getIScene() const = 0;

		/* Checks if pointed towards a valid Entity ID. */
		virtual bool isValid() = 0;
		/* Checks if the Handle is pointing to the correct location in memory. */
		virtual bool isUpdated() = 0;
		/* Returns true if the Handle is valid. If not, the handle is updated and returns true if the update was successful. */
		virtual bool refresh() = 0;
		/* Makes the Handle invalid. */
		virtual void clear() = 0;
		/* Updates the Handle so it points towards the target Entity. */
		virtual void update() = 0;
		/* Returns the number of parents above in the hierarchy. */
		virtual std::size_t getDepth() = 0;

		virtual void addComponent(ComponentTypeID componentTypeID) = 0;
		virtual void removeComponent(ComponentTypeID componentTypeID) = 0;

		virtual bool hasComponent(ComponentTypeID typeID) = 0;
		virtual IComponent* getIComponent(ComponentTypeID typeID) = 0;
		virtual void getIComponents(IComponent** out, std::size_t count) = 0;
		virtual void getIComponents(ComponentTypeID typeID, IComponent** out, std::size_t count) = 0;
		virtual std::size_t getComponentCount() = 0;
		virtual std::size_t getComponentCount(ComponentTypeID typeID) = 0;

		virtual const char* getEntityName() = 0;
		virtual bool renameEntity(const char* name) = 0;
		virtual HideFlags getEntityHideFlags() = 0;
		virtual void setEntityHideFlags(HideFlags hideFlags) = 0;

		virtual bool hasParent() = 0;
		virtual void setParent(const Entity& entity, bool keepPosition = false) = 0;
		virtual void removeParent(bool keepPosition = false) = 0;
		virtual Entity getParentEntity() = 0;
		virtual Entity getChildEntity(std::size_t index) = 0;
		//virtual Handle createIParentHandle() = 0;
		//virtual Handle createIChildHandle(std::size_t index) = 0;
		// TODO: createIParentHandle
		// TODO: createIChildHandle
		virtual std::size_t getImmediateChildCount() = 0;
		virtual std::size_t getChildCount() = 0;

		virtual void setSiblingIndex(std::size_t index) = 0;
		virtual void setSiblingIndexQueued(std::size_t index) = 0;
		virtual std::size_t getSiblingIndex() = 0;
	};
}