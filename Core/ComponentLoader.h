#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "EntityHandle.h"
#include "ComponentTypeInfo.h"
#include "ComponentType.h"
#include "ParentEntity.h"
#include <string>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#define REGISTER_LOADABLE_COMPONENT(T) Core::ComponentLoader::registerComponent<T>()
#define REGISTER_IGNORE_ON_LOAD_COMPONENT(T) Core::ComponentLoader::registerIgnore<T>();

namespace Core {

	class ComponentLoader {
	public:
		ComponentLoader();
		~ComponentLoader();

		/* Loads Component from the given instream and adds it to the given EntityHandle. */
		static void addComponentFromFile(std::istream& is, std::string componentTypeName, EntityHandle& go);
		template<typename T> static bool registerIgnore();
		template<typename T> static void registerComponent();

	private:
		template<typename T> static void addComponentFromFile_impl(std::istream& is, EntityHandle& go);

	private:
		typedef void(*LoaderFunc)(std::istream&, EntityHandle&);
		static std::map<std::string, LoaderFunc>& getLoaderMap() {
			static std::map<std::string, LoaderFunc> loaderMap;
			return loaderMap;
		}
		static std::vector<std::string>& getIgnoreList() {
			static std::vector<std::string> ignoreList;
			return ignoreList;
		}
	};

	// --------------------------- Template Function Definitions --------------------------------

	template<typename T>
	void ComponentLoader::registerComponent() {
		LoaderFunc fun = &ComponentLoader::addComponentFromFile_impl<T>;
		getLoaderMap()[nameof(T)] = fun;
	}
	
	template<typename T>
		bool ComponentLoader::registerIgnore() {
		getIgnoreList().push_back(nameof(T));
		return true;
	}
	template<typename T>
	void ComponentLoader::addComponentFromFile_impl(std::istream& is, EntityHandle& go) {
		if (!go.refresh()) {
			std::cout << "ComponentLoader::addComponentFromFile_impl<T>::ERROR Could not add component to invalid EntityHandle!\n";
			throw std::invalid_argument("ComponentLoader::addComponentFromFile_impl<T>::ERROR Could not add component to invalid EntityHandle!");
		}

		T loadedComponent;
		//char loadedComponent[sizeof(T)];
		//is.read((char*)&loadedComponent, sizeof(T));

		try {
			loadedComponent.deserialize(is);
			if (typeof(ParentEntity) != typeof(T)) // ParentEntity Component is loaded differently
				go.addComponent(loadedComponent);	// Add
		}
		catch (std::exception& e) {
			std::cout << "ComponentLoader::addComponentFromFile_impl<T>::ERROR An Exception was thrown when loading a component!\n";
			std::cout << "Exception: " << e.what() << "\n";
			throw;
		}
	}
}
#endif