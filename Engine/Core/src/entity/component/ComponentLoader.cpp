#include "ComponentLoader.h"

using namespace Core;

ComponentLoader::ComponentLoader()
{
}


ComponentLoader::~ComponentLoader()
{
}

void ComponentLoader::addComponentFromFile(DeserializerArchive& ar, std::string componentTypeName, EntityHandle& go) {
	auto it = getLoaderMap().find(componentTypeName);
	if (it == getLoaderMap().end()) {
		std::cout << "ComponentLoader::addComponentFromFile::WARNING Skipping unknown component type (" << componentTypeName << ") read from file\n";
		// Skip component
		ar.skipProperties();
		return;
	}

	LoaderFunc fun = it->second;
	fun(ar, go);
}

void ComponentLoader::updateComponentFromFile(PrefabDeserializerArchive& ar, std::string componentTypeName, EntityHandle& go) {
	auto it = getUpdaterMap().find(componentTypeName);
	if (it == getUpdaterMap().end()) {
		std::cout << "ComponentLoader::updateComponentFromFile::WARNING Skipping unknown component type (" << componentTypeName << ") read from file\n";
		// Skip component
		ar.skipProperties();
		return;
	}

	UpdaterFunc fun = it->second;
	fun(ar, go);
}