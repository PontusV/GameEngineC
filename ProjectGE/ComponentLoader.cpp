#include "ComponentLoader.h"

using namespace Core;


ComponentLoader::ComponentLoader()
{
}


ComponentLoader::~ComponentLoader()
{
}

void ComponentLoader::addComponentFromFile(std::istream& is, EntityHandle& go) {
	ComponentTypeID typeID;
	is.read((char*)&typeID, sizeof(ComponentTypeID));

	auto it = getLoaderMap().find(typeID);
	if (it == getLoaderMap().end()) {
		// Check if type is in ignore list
		std::vector<ComponentTypeID>& ignoreList = getIgnoreList();
		for (std::size_t i = 0; i < ignoreList.size(); i++) {
			if (typeID == ignoreList[i]) return; // Ignored
		}
		std::cout << "ComponentLoader::loadComponentFromFile::ERROR Invalid typeID read from file. The component might not be registered or the next variable in file may not have been a ComponentTypeID.\n";
		throw std::invalid_argument("ComponentLoader::loadComponentFromFile::ERROR Invalid typeID read from file. The component might be unregistered or the next variable in file may not have been a ComponentTypeID.");
	}

	LoaderFunc fun = it->second;
	fun(is,go);
}