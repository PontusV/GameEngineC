#pragma once
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <algorithm>
#include "EngineDLLInterface.h"
#undef min

namespace Editor {
	class EngineDLL;

	struct SerializedProperty {
		std::size_t entityID;
		std::string typeName;
		std::string propertyName;
		std::string serializedData;
	};

	struct SerializedComponent {
		std::size_t entityID;
		std::string typeName;
		std::string serializedData;
	};

	class DoAction {
	public:
		virtual ~DoAction() = 0;
		virtual std::unique_ptr<DoAction> call(EngineDLL* engineDLL) = 0;
		std::size_t getRootEntityID() { return rootEntityID; }
		virtual bool isValid(EngineDLL* engineDLL) = 0;
	protected:
		DoAction(std::size_t rootEntityID) : rootEntityID(rootEntityID) {}
		std::size_t rootEntityID;
	};

	/* Manages the Undo and Redo stacks of root prefabs */
	class UndoRedoManager {
	public:
		UndoRedoManager(EngineDLL* engineDLL) : engineDLL(engineDLL) {}
		~UndoRedoManager() {}

		bool undo();
		bool redo();
		void enable();
		void disable();
		/* Clears both undo and redo stack */
		void clearStack();
		/* Clears both undo and redo stack for the given Entity */
		void removeEntityFromStack(std::size_t rootEntityID);
		void registerUndo(std::unique_ptr<DoAction>&& action);
		void registerRedo(std::unique_ptr<DoAction>&& action);

		bool isUndoAvailable();
		bool isRedoAvailable();

		void resetStepsSinceSave(std::size_t rootEntityID);
		int getStepsSinceSave(std::size_t rootEntityID);

		bool isUnsaved(std::size_t rootEntityID);
		void setDirty(std::size_t rootEntityID, bool value);

	private:
		bool enabled = true;
		std::vector<std::unique_ptr<DoAction>> undoStack;
		std::vector<std::unique_ptr<DoAction>> redoStack;
		EngineDLL* engineDLL;
		std::map<std::size_t, int> stepsSinceSave; // EntityID, steps
		std::vector<std::size_t> dirtyRootEntityIDs; // Marked as unsaved until next save
	};

	// -------------------- Actions --------------------

	class CreateEntityAction : public DoAction {
	public:
		CreateEntityAction(std::size_t rootEntityID, std::size_t entityID, std::string&& serializedEntityData) : DoAction(rootEntityID), entityID(entityID), serializedEntityData(serializedEntityData) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::string serializedEntityData;
	};

	class DestroyEntityAction : public DoAction {
	public:
		DestroyEntityAction(std::size_t rootEntityID, std::size_t entityID) : DoAction(rootEntityID), entityID(entityID) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
	};

	class AddComponentAction : public DoAction {
	public:
		AddComponentAction(std::size_t rootEntityID, std::size_t entityID, std::string typeName, std::string&& serializedComponentData, bool overriden) : DoAction(rootEntityID), entityID(entityID), typeName(typeName), serializedComponentData(serializedComponentData), prevOverriden(overriden) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::string typeName;
		std::string serializedComponentData;
		bool prevOverriden;
	};

	class RemoveComponentAction : public DoAction {
	public:
		RemoveComponentAction(std::size_t rootEntityID, std::size_t entityID, std::string typeName, bool overriden) : DoAction(rootEntityID), entityID(entityID), typeName(typeName), prevOverriden(overriden) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::string typeName;
		bool prevOverriden;
	};

	class MoveEntityAction : public DoAction {
	public:
		MoveEntityAction(std::size_t rootEntityID, std::size_t entityID, float x, float y) : DoAction(rootEntityID), entityID(entityID), x(x), y(y) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		float x;
		float y;
	};

	class RenameEntityAction : public DoAction {
	public:
		RenameEntityAction(std::size_t rootEntityID, std::size_t entityID, std::string newName) : DoAction(rootEntityID), entityID(entityID), name(newName) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::string name;
	};

	class SetEntityParentAction : public DoAction {
	public:
		SetEntityParentAction(std::size_t rootEntityID, std::size_t entityID, std::size_t parentEntityID) : DoAction(rootEntityID), entityID(entityID), parentEntityID(parentEntityID) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::size_t parentEntityID;
	};

	class PropertyAssignAction : public DoAction {
	public:
		PropertyAssignAction(std::size_t rootEntityID, std::size_t entityID, std::string typeName, std::string propertyName, std::string&& serializedPropertyData, bool overriden) : DoAction(rootEntityID), entityID(entityID), typeName(typeName), propertyName(propertyName), serializedPropertyData(serializedPropertyData), prevOverriden(overriden) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::string typeName;
		std::string propertyName;
		std::string serializedPropertyData;
		bool prevOverriden;
	};

	class UnpackPrefabAction : public DoAction { // WIP
	public:
		UnpackPrefabAction(std::size_t rootEntityID, std::size_t entityID) : DoAction(rootEntityID), entityID(entityID) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
	};

	class RepackPrefabAction : public DoAction { // WIP
	public:
		RepackPrefabAction(std::size_t rootEntityID, std::size_t entityID, std::string&& serializedComponentData, std::vector<PropertyOverride> propertyOverrides, std::vector<ComponentOverride> componentOverrides) : DoAction(rootEntityID), entityID(entityID), serializedComponentData(serializedComponentData), propertyOverrides(propertyOverrides), componentOverrides(componentOverrides) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::string serializedComponentData;
		std::vector<PropertyOverride> propertyOverrides;
		std::vector<ComponentOverride> componentOverrides;
	};

	class RevertPrefabAction : public DoAction { // WIP
	public:
		RevertPrefabAction(std::size_t rootEntityID, std::size_t entityID) : DoAction(rootEntityID), entityID(entityID) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
	};

	class UndoRevertPrefabAction : public DoAction { // WIP
	public:
		UndoRevertPrefabAction(std::size_t rootEntityID, std::size_t entityID, std::vector<SerializedProperty> serializedProperties, std::vector<SerializedComponent> serializedComponents) : DoAction(rootEntityID), entityID(entityID), serializedProperties(serializedProperties), serializedComponents(serializedComponents) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
		bool isValid(EngineDLL* engineDLL) override;
	private:
		std::size_t entityID;
		std::vector<SerializedProperty> serializedProperties;
		std::vector<SerializedComponent> serializedComponents;
	};
}