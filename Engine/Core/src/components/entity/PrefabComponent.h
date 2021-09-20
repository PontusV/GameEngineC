#ifndef PREFAB_COMPONENT_H
#define PREFAB_COMPONENT_H

#include "entity/component/IComponentData.h"
#include "entity/handle/ComponentHandle.h"
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "PrefabComponent.generated.h"

namespace Core {

	/* Holds data for Entity connected to a prefab*/
	struct ConnectedEntity {
		template<typename Archive>
		void serialize(Archive& ar) const {
			ar(guid, entity, componentTypeIDs);
		}

		template<typename Archive>
		void deserialize(Archive& ar) {
			ar(guid, entity, componentTypeIDs);
		}

		std::string guid; // The GUID used when loading
		Entity entity;
		std::vector<std::size_t> componentTypeIDs; // Components connected to the prefab
	};

	struct PrefabPropertyOverride {
	public:
		PrefabPropertyOverride() {}
		PrefabPropertyOverride(ComponentHandle handle, std::string propertyName) : targetComponent(handle), targetPropertyName(propertyName) {}

		template<typename Archive>
		void serialize(Archive& ar) const {
			ar(targetComponent, targetPropertyName);
		}

		template<typename Archive>
		void deserialize(Archive& ar) {
			ar(targetComponent, targetPropertyName);
		}

		ComponentHandle targetComponent;
		std::string targetPropertyName;
	};

	/* Gives an Entity a reference to a Prefab. Identifies a Prefab */
	CLASS(hideInInspector) PrefabComponent : public IComponentData {
		GENERATED_BODY()
	public:
		PrefabComponent(std::string filePath);
		PrefabComponent();
		~PrefabComponent();

		bool isDirty() const;
		void setDirty(bool value);

		void setFilePath(std::string filePath);
		std::string getFilePath();

		/* Sets property as an override. This means the property will not be loaded from prefab. Returns true if the given property exists and was successfully added as an override */
		bool addPropertyOverride(PrefabPropertyOverride prefabPropertyOverride);
		bool removePropertyOverride(PrefabPropertyOverride prefabPropertyOverride);
		std::vector<PrefabPropertyOverride> getPropertyOverrides() const;

		void setConnectedEntities(std::vector<ConnectedEntity> value);
		std::vector<ConnectedEntity> getConnectedEntities() const;

		/* Removes all overrides which no longer have a valid target */
		void removeDanglingOverrides();

	private:
		bool dirty = true; // Determines if the prefab requires an update or not
		PROPERTY(Category=PrefabPath)
		std::string filePath;
		PROPERTY(alwaysOverriden)
		std::vector<PrefabPropertyOverride> prefabPropertyOverrides; // Blocks load of property from prefab
		PROPERTY(alwaysOverriden)
		std::vector<ConnectedEntity> connectedEntities;
	};
}
#endif