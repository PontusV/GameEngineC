#include "ComponentLoader.h"

using namespace Core;

ComponentLoader::ComponentLoader()
{
}


ComponentLoader::~ComponentLoader()
{
}

void ComponentLoader::addComponentFromFile(std::istream& is, std::string componentTypeName, EntityHandle& go) {
	auto it = getLoaderMap().find(componentTypeName);
	if (it == getLoaderMap().end()) {
		// Check if type is in ignore list
		std::vector<std::string>& ignoreList = getIgnoreList();
		for (std::size_t i = 0; i < ignoreList.size(); i++) {
			if (componentTypeName == ignoreList[i]) return; // Ignored
		}
		std::cout << "ComponentLoader::loadComponentFromFile::ERROR Invalid type (" << componentTypeName << ") read from file. The component might not be registered or the next variable in file may not have been a Component.\n";
		throw std::invalid_argument("ComponentLoader::loadComponentFromFile::ERROR Invalid typeID read from file. The component might be unregistered or the next variable in file may not have been a Component.");
	}

	LoaderFunc fun = it->second;
	fun(is, go);
}