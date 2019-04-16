#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H
namespace Core {
	class ComponentRegistry {
	public:
		/* Registers all Component Types from ComponentMetaInfo.h */
		static bool registerComponentTypes();
	};
}

#endif