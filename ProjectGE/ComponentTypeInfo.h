#ifndef COMPONENT_TYPE_INFO_H
#define COMPONENT_TYPE_INFO_H

#include <cstddef>

namespace Core {
	typedef std::size_t ComponentTypeID;

	struct ComponentTypeInfo {
		ComponentTypeInfo(std::size_t size, ComponentTypeID typeID) : size(size), id(typeID) {}
		ComponentTypeInfo(const ComponentTypeInfo& other) {
			size = other.size;
			id = other.id;
		}
		ComponentTypeInfo& operator=(const ComponentTypeInfo& other) {
			size = other.size;
			id = other.id;
			return *this;
		}
		bool operator==(const ComponentTypeInfo& other) {
			return id == other.id;
		}
		std::size_t size;
		ComponentTypeID id;
	};
}
#endif