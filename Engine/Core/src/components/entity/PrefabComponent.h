#ifndef PREFAB_COMPONENT_H
#define PREFAB_COMPONENT_H

#include "entity/component/IComponentData.h"
#include "entity/handle/ComponentHandle.h"
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include "PrefabComponent.generated.h"

namespace Core {

	struct PrefabPropertyOverride {
		PrefabPropertyOverride() {}
		PrefabPropertyOverride(ComponentHandle handle, std::string propertyName) : targetComponent(handle), targetPropertyName(propertyName) {}

		template<typename Archive>
		void serialize(Archive& archive) const {
			archive(targetComponent, targetPropertyName);
		}

		template<typename Archive>
		void deserialize(Archive& archive) {
			archive(targetComponent, targetPropertyName);
		}

		ComponentHandle targetComponent;
		std::string targetPropertyName;
	};

	struct PrefabComponentOverride {
		PrefabComponentOverride() {}
		PrefabComponentOverride(Handle handle, std::size_t typeID) : targetEntity(handle), typeID(typeID) {}

		template<typename Archive>
		void serialize(Archive& archive) const {
			archive(targetEntity, typeID);
		}

		template<typename Archive>
		void deserialize(Archive& archive) {
			archive(targetEntity, typeID);
		}

		Handle targetEntity;
		std::size_t typeID;
	};

	/* Gives an Entity a reference to a Prefab. Identifies a Prefab */
	CLASS(hideInInspector) PrefabComponent : public IComponentData {
		GENERATED_BODY()
	public:
		PrefabComponent(std::string filePath);
		PrefabComponent();
		~PrefabComponent();

		bool isDirty() const;
		bool setDirty(bool value);

		void setFilePath(std::string filePath);
		std::string getFilePath();

		/* Sets property as an override. This means the property will not be loaded from prefab. Returns true if the given property exists and was successfully added as an override */
		bool addPropertyOverride(PrefabPropertyOverride prefabPropertyOverride);
		bool removePropertyOverride(PrefabPropertyOverride prefabPropertyOverride);
		std::vector<PrefabPropertyOverride> getPropertyOverrides() const;

		/* Sets property as an override. This means the component will not be loaded from prefab. Returns true if the override was successfully added */
		bool addComponentOverride(PrefabComponentOverride prefabComponentOverride);
		bool removeComponentOverride(PrefabComponentOverride prefabComponentOverride);
		std::vector<PrefabComponentOverride> getComponentOverrides() const;

		void setEntityRemapInfo(std::map<std::string, std::size_t> entityRemapInfo);
		std::map<std::string, std::size_t> getEntityRemapInfo() const;

		/* Removes all overrides which no longer have a valid target */
		void removeDanglingOverrides();

	private:
		bool dirty = true; // Determines if the prefab requires an update or not
		PROPERTY(Category=PrefabPath)
		std::string filePath;
		PROPERTY()
		std::vector<PrefabPropertyOverride> prefabPropertyOverrides; // Blocks load of property from prefab
		PROPERTY()
		std::vector<PrefabComponentOverride> prefabComponentOverrides; // Blocks load of component from prefab
		PROPERTY()
		std::map<std::string, Entity> entityRemapInfo;
	};
}
#endif