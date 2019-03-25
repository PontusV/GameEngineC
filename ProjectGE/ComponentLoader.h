#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "Level.h"
#include "EntityHandle.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#define REGISTER_LOADABLE_COMPONENT(T) bool isRegistered_##T = Core::ComponentLoader::registerComponent<T>()
#define REGISTER_LOAD_COMPONENT_IGNORE(T) bool isIgnored_##T = Core::ComponentLoader::registerIgnore<T>();

namespace Core {

	class ComponentLoader {
	public:
		ComponentLoader();
		~ComponentLoader();

		/* Loads Component from the given instream and adds it to the given EntityHandle. */
		static void							addComponentFromFile(std::istream& is, EntityHandle& go);
		template<typename T> static bool	registerComponent();
		template<typename T> static bool	registerIgnore();

	private:
		template<typename T> static void	addComponentFromFile_impl(std::istream& is, EntityHandle& go);

	private:
		typedef void(*LoaderFunc)(std::istream&, EntityHandle&);
		static std::map<ComponentTypeID, LoaderFunc>& getLoaderMap() {
			static std::map<ComponentTypeID, LoaderFunc> loaderMap;
			return loaderMap;
		}
		static std::vector<ComponentTypeID>& getIgnoreList() {
			static std::vector<ComponentTypeID> ignoreList;
			return ignoreList;
		}
	};

	// --------------------------- Template Function Definitions --------------------------------

	template<typename T>
	bool ComponentLoader::registerComponent() {
		LoaderFunc fun = &ComponentLoader::addComponentFromFile_impl<T>;
		getLoaderMap()[T::TYPE_ID] = fun;
		//std::cout << typeid(T).name() << "\tregistered\t Total registered: " << getLoaderMap().size() << "\n";
		return true;
	}
	
	template<typename T>
		bool ComponentLoader::registerIgnore() {
		getIgnoreList().push_back(T::TYPE_ID);
		//std::cout << typeid(T).name() << "\tignored\t Total ignored: " << getLoaderMap().size() << "\n";
		return true;
	}

	template<typename T>
	void ComponentLoader::addComponentFromFile_impl(std::istream& is, EntityHandle& go) {
		if (!go.isValid()) {
			std::cout << "ComponentLoader::addComponentFromFile_impl<T>::ERROR Could not add component to invalid EntityHandle!\n";
			throw std::invalid_argument("ComponentLoader::addComponentFromFile_impl<T>::ERROR Could not add component to invalid EntityHandle!");
		}

		T* loadedComponent = new T();
		try {
			loadedComponent->deserialize(is);	// Load
			go.addComponent(loadedComponent);	// Add
		}
		catch (std::exception& e) {
			delete loadedComponent;				// Clean up
			std::cout << "ComponentLoader::addComponentFromFile_impl<T>::ERROR An Exception was thrown when loading a component!\n";
			std::cout << "Exception: " << e.what() << "\n";
			throw;
		}
	}
}
#endif