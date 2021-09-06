#ifndef COMPONENT_LOADER_H
#define COMPONENT_LOADER_H

#include "engine/Serializer.h"
#include "entity/handle/EntityHandle.h"
#include "ComponentTypeInfo.h"
#include "ComponentType.h"
#include "components/entity/ParentEntity.h"
#include <string>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#define REGISTER_LOADABLE_COMPONENT(T) Core::ComponentLoader::registerComponent<T>()

namespace Core {

	class ComponentLoader {
	public:
		ComponentLoader();
		~ComponentLoader();

		/* Loads Component from the given instream and adds it to the given EntityHandle. */
		static void addComponentFromFile(DeserializerArchive& ar, std::string componentTypeName, EntityHandle& go);
		static void updateComponentFromFile(PrefabDeserializerArchive& ar, std::string componentTypeName, EntityHandle& go);
		template<typename T> static void registerComponent();

	private:
		template<typename T> static void addComponentFromFile_impl(DeserializerArchive& archive, EntityHandle& go);
		template<typename T> static void updateComponentFromFile_impl(PrefabDeserializerArchive& archive, EntityHandle& go);

	private:
		typedef void(*LoaderFunc)(DeserializerArchive&, EntityHandle&);
		static std::map<std::string, LoaderFunc>& getLoaderMap() {
			static std::map<std::string, LoaderFunc> loaderMap;
			return loaderMap;
		}
		typedef void(*UpdaterFunc)(PrefabDeserializerArchive&, EntityHandle&);
		static std::map<std::string, UpdaterFunc>& getUpdaterMap() {
			static std::map<std::string, UpdaterFunc> updaterMap;
			return updaterMap;
		}
	};

	// --------------------------- Template Function Definitions --------------------------------

	template<typename T>
	void ComponentLoader::registerComponent() {
		LoaderFunc addFun = &ComponentLoader::addComponentFromFile_impl<T>;
		getLoaderMap()[nameof(T)] = addFun;
		UpdaterFunc updateFun = &ComponentLoader::updateComponentFromFile_impl<T>;
		getUpdaterMap()[nameof(T)] = updateFun;
	}
	
	template<typename T>
	void ComponentLoader::addComponentFromFile_impl(DeserializerArchive& ar, EntityHandle& go) {
		if (!go.refresh()) {
			std::cout << "ComponentLoader::addComponentFromFile_impl<T>::ERROR Could not add component to invalid EntityHandle!\n";
			throw std::invalid_argument("ComponentLoader::addComponentFromFile_impl<T>::ERROR Could not add component to invalid EntityHandle!");
		}

		T loadedComponent;

		try {
			loadedComponent.deserialize(ar);
			go.addComponent(loadedComponent);
		}
		catch (std::exception& e) {
			std::cout << "ComponentLoader::addComponentFromFile_impl<T>::ERROR An Exception was thrown when loading a component!\n";
			std::cout << "Exception: " << e.what() << "\n";
			throw;
		}
	}

	template<typename T>
	void ComponentLoader::updateComponentFromFile_impl(PrefabDeserializerArchive& ar, EntityHandle& go) {
		if (!go.refresh()) {
			std::cout << "ComponentLoader::updateComponentFromFile_impl<T>::ERROR Could not update component of invalid EntityHandle!\n";
			throw std::invalid_argument("ComponentLoader::updateComponentFromFile_impl<T>::ERROR Could not update component of invalid EntityHandle!");
		}

		T* component = go.getComponent<T>();

		if (component == nullptr) {
			std::cout << "ComponentLoader::updateComponentFromFile_impl<T>::ERROR Cannot update non-existant component!" << std::endl;
			throw "ComponentLoader::updateComponentFromFile_impl<T>::ERROR Cannot update non-existant component!";
		}

		try {
			component->deserialize(ar);
		}
		catch (std::exception& e) {
			std::cout << "ComponentLoader::updateComponentFromFile_impl<T>::ERROR An Exception was thrown when loading a component!\n";
			std::cout << "Exception: " << e.what() << "\n";
			throw;
		}
	}
}
#endif