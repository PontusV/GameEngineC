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
		std::cout << "ComponentLoader::loadComponentFromFile::WARNING Skipping unknown component type (" << componentTypeName << ") read from file\n";
		// Skip component
		Mirror::skipProperties(is);
		return;
	}

	LoaderFunc fun = it->second;
	fun(is, go);
}