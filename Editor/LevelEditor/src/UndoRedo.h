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

	class DoAction {
	public:
		virtual ~DoAction() = 0;
		virtual std::unique_ptr<DoAction> call(EngineDLL* engineDLL) = 0;
		std::size_t getSceneIndex() { return sceneIndex; }
	protected:
		DoAction(std::size_t sceneIndex) : sceneIndex(sceneIndex) {}
		std::size_t sceneIndex;
	};

	/* Manages Undo and Redo stacks for scenes */
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
		/* Clears both undo and redo stack for the given scene */
		void removeSceneFromStack(std::size_t sceneIndex);
		void registerUndo(std::unique_ptr<DoAction>&& action);
		void registerRedo(std::unique_ptr<DoAction>&& action);

		bool isUndoAvailable();
		bool isRedoAvailable();

		void resetStepsSinceSave(std::size_t sceneIndex);
		int getStepsSinceSave(std::size_t sceneIndex);

	private:
		bool enabled = true;
		std::vector<std::unique_ptr<DoAction>> undoStack;
		std::vector<std::unique_ptr<DoAction>> redoStack;
		EngineDLL* engineDLL;
		std::map<std::size_t, int> stepsSinceSave; // scenIndex, steps
	};

	// -------------------- Actions --------------------

	struct IPropertyValueData {
	public:
		~IPropertyValueData() {}
		virtual std::shared_ptr<IPropertyValueData> assignValueTo(ReflectedFieldData& field) = 0;
		virtual std::size_t getMemSize() = 0;
		virtual bool isTriviallyCopyable() = 0;
	protected:
		IPropertyValueData() {}
	};

	template<typename T>
	struct TPropertyValueData : public IPropertyValueData {
	public:
		TPropertyValueData(T& data) : data(data) {}
		std::shared_ptr<IPropertyValueData> assignValueTo(ReflectedFieldData& field) override {
			return assignValueToImpl(field);
		}
		std::size_t getMemSize() override {
			return sizeof(T);
		}
		bool isTriviallyCopyable() override {
			return std::is_trivially_copyable<T>::value;
		}
	private:
		template<class Q = T>
		typename std::enable_if<std::is_trivially_copyable<Q>::value, std::shared_ptr<IPropertyValueData>>::type assignValueToImpl(ReflectedFieldData& field) {
			std::shared_ptr<IPropertyValueData> prevData = std::make_shared<TPropertyValueData<Q>>(*static_cast<Q*>(field.ptr));
			std::memset(field.ptr, 0, field.size);
			std::memcpy(field.ptr, &data, std::min(field.size, sizeof(data)));
			return prevData;
		}
		template<class Q = T>
		typename std::enable_if<!std::is_trivially_copyable<Q>::value, std::shared_ptr<IPropertyValueData>>::type assignValueToImpl(ReflectedFieldData& field) {
			assert(sizeof(Q) == field.size);
			std::shared_ptr<IPropertyValueData> prevData = std::make_shared<TPropertyValueData<Q>>(*static_cast<Q*>(field.ptr));
			*static_cast<Q*>(field.ptr) = data;
			return prevData;
		}
	private:
		T data;
	};

	template<typename T>
	std::shared_ptr<IPropertyValueData> createPropValueData(T& data) {
		return std::make_shared<TPropertyValueData<T>>(data);
	}

	template<typename... Ts>
	std::vector<std::shared_ptr<IPropertyValueData>> createPropDataFromValues(Ts... data) {
		return { std::make_shared<TPropertyValueData<Ts>>(data)... };
	}

	std::vector<std::shared_ptr<IPropertyValueData>> createPropData(ReflectedPropertyData& prop);

	struct PropertyBlueprint {
		std::string name;
		std::string typeName;
		std::vector<std::shared_ptr<IPropertyValueData>> propertyValues;
	};

	struct ComponentBlueprint {
		static ComponentBlueprint createFromComponent(EngineDLL* engineDLL, ComponentData componentData);
		std::string typeName;
		std::vector<PropertyBlueprint> propertyBPs;
	};

	struct EntityBlueprint {
		static EntityBlueprint createFromEntity(EngineDLL* engineDLL, EntityID entityID);
		static EntityID createEntityFromBlueprint(EngineDLL* engineDLL, EntityBlueprint blueprint, std::size_t sceneIndex);
		std::string entityName;
		std::vector<ComponentBlueprint> componentBPs;
		std::vector<EntityBlueprint> children;
	};

	// ---

	class CreateEntityAction : public DoAction { // WIP
	public:
		CreateEntityAction(std::size_t sceneIndex, std::string entityName, EntityBlueprint&& blueprint) : DoAction(sceneIndex), blueprint(blueprint) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		EntityBlueprint blueprint;
	};

	class DestroyEntityAction : public DoAction { // WIP
	public:
		DestroyEntityAction(std::size_t sceneIndex, std::string entityName) : DoAction(sceneIndex), entityName(entityName) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		std::string entityName;
	};

	class AddComponentAction : public DoAction { // WIP
	public:
		AddComponentAction(std::size_t sceneIndex, std::string entityName, std::string typeName, ComponentBlueprint&& blueprint) : DoAction(sceneIndex), entityName(entityName), typeName(typeName), blueprint(blueprint) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		std::string entityName;
		std::string typeName;
		ComponentBlueprint blueprint;
	};

	class RemoveComponentAction : public DoAction { // WIP
	public:
		RemoveComponentAction(std::size_t sceneIndex, std::string entityName, std::string typeName) : DoAction(sceneIndex), entityName(entityName), typeName(typeName) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		std::string entityName;
		std::string typeName;
	};

	class MoveEntityAction : public DoAction { // WIP
	public:
		MoveEntityAction(std::size_t sceneIndex, std::string entityName, float x, float y) : DoAction(sceneIndex), entityName(entityName), x(x), y(y) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		std::string entityName;
		float x;
		float y;
	};

	class RenameEntityAction : public DoAction { // WIP
	public:
		RenameEntityAction(std::size_t sceneIndex, std::string entityName, std::string newName) : DoAction(sceneIndex), entityName(entityName), name(newName) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		std::string entityName;
		std::string name;
	};

	class SetEntityParentAction : public DoAction { // WIP
	public:
		SetEntityParentAction(std::size_t sceneIndex, std::string entityName, std::string prevParentName) : DoAction(sceneIndex), entityName(entityName), parentName(prevParentName) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		std::string entityName;
		std::string parentName;
	};

	class PropertyAssignAction : public DoAction { // WIP
	public:
		PropertyAssignAction(std::size_t sceneIndex, std::string entityName, std::string typeName, std::string propertyName, std::vector<std::shared_ptr<IPropertyValueData>>&& propertyValues) : DoAction(sceneIndex), entityName(entityName), typeName(typeName), propertyName(propertyName), propertyValues(propertyValues) {}
		std::unique_ptr<DoAction> call(EngineDLL* engineDLL) override;
	private:
		std::string entityName;
		std::string typeName;
		std::string propertyName;
		std::vector<std::shared_ptr<IPropertyValueData>> propertyValues;
	};
}