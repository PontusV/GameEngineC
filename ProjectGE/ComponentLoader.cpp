#include "ComponentLoader.h"

using namespace Core;

// Register all components
#include "Button.h"
REGISTER_LOADABLE_COMPONENT(Button);
#include "Image.h"
REGISTER_LOADABLE_COMPONENT(Image);
#include "Panel.h"
REGISTER_LOADABLE_COMPONENT(Panel);
#include "Rect.h"
REGISTER_LOADABLE_COMPONENT(Rect);
#include "Text.h"
REGISTER_LOADABLE_COMPONENT(Text);
#include "Transform.h"
REGISTER_LOADABLE_COMPONENT(Transform);
#include "Border.h"
REGISTER_LOADABLE_COMPONENT(Border);
#include "ParentEntity.h"
REGISTER_LOAD_COMPONENT_IGNORE(ParentEntity);
#include "ChildManager.h"
REGISTER_LOAD_COMPONENT_IGNORE(ChildManager);
// End of registry

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
		std::cout << "ComponentLoader::loadComponentFromFile::ERROR Invalid typeID(" << typeID << ") read from file. The component might not be registered or the next variable in file may not have been a ComponentTypeID.\n";
		throw std::invalid_argument("ComponentLoader::loadComponentFromFile::ERROR Invalid typeID read from file. The component might be unregistered or the next variable in file may not have been a ComponentTypeID.");
	}

	LoaderFunc fun = it->second;
	fun(is,go);
}