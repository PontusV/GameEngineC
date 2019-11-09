#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "components/Behaviour.h"
#include "entity/handle/EntityHandle.h"
#include "PropertyEditor.h"
#include <ReflectionParser/ReflectionTypes.h>
#include "PropertyValueID.h"
#include "ReflectedObjectHandle.h"
#include "Inspector.generated.h"

namespace Core {

	class ReflectedObject;
	class Selectable;

	struct PropertyInstance {
		std::size_t lineIndex;
		Component* instance;
		Mirror::Property prop;
	};

	CLASS(hideInInspector) Inspector : public Behaviour {
		GENERATED_BODY()
	public:
		Inspector();
		~Inspector();

		void onEnable() override;
		void onDisable() override;
		void lateUpdate(float deltaTime) override;

		void inspect(EntityHandle entity);
		/* Recreates target component list if target exists. */
		void refresh();

		template<typename T>
		void addComponentToTarget();
		void removeComponentFromTarget(ComponentTypeID typeID);

		void collapse(EntityHandle contentHandle, EntityHandle iconHandle);

	private:
		void createEntries();
		void clearEntries();

		void addComponentEntry(Component* component, std::size_t id);
		EntityHandle createPropertyField(std::string name, Mirror::Property& prop, Component* component, Mirror::Property& rootProp, void* instance, std::size_t typeID);
		EntityHandle createPropertyValueField(std::string label, PropertyValueID value, Component* root, Mirror::Property& rootProp, void* instance, std::size_t typeID, std::string entityName);

		void setNextSelectable(const EntityHandle& entity);
		EntityHandle firstSelectable;
		EntityHandle prevSelectable;
	private:
		EntityHandle currentTarget;
		std::vector<EntityHandle> targetComponentList;
		std::vector<ComponentTypeID> targetComponents;
	};

	template<typename T>
	void Inspector::addComponentToTarget() {
		currentTarget.addComponent<T>();
	}
}
#endif