#ifndef COMPONENT_TYPE_INFO_H
#define COMPONENT_TYPE_INFO_H

#include "ComponentType.h"
#include <cstddef>
#include <vector>
#include <string>
#include <iostream>

#define typeof(T) Core::ComponentTypeInfo<T>::getType()
#define typeIDof(T) Core::ComponentTypeInfo<T>::getTypeID()
#define nameof(T) Core::ComponentTypeInfo<T>::getName()

namespace Core {
	struct IComponentTypeInfo {
	public:
		IComponentTypeInfo(ComponentType type, const char* name, std::size_t size) : type(type), name(name), size(size) {}

		bool operator==(const ComponentTypeID& other) {
			return type.getTypeID() == other;
		}

		ComponentType type;
		std::size_t size;
		std::string name;
	};

	template<typename T>
	class ComponentTypeInfo {
	public:
		static void setMatchingTypeIDs(std::vector<ComponentTypeID> typeIDs) {
			// Put the ComponentTypeID of T to front
			for (std::size_t i = 0; i < typeIDs.size(); i++) {
				if (typeIDs[i] == id) {
					//Swap
					ComponentTypeID temp = typeIDs[0];
					typeIDs[0] = typeIDs[i];
					typeIDs[i] = temp;
					break;
				}
			}
			type = ComponentType(typeIDs);
		}

		static void setTypeID(ComponentTypeID value) {
			id = value;
		}

		static ComponentTypeID getTypeID() {
			return id;
		}

		static const ComponentType& getType() {
			return type;
		}

		static const char* getName() { // Used for parameter packs
			return T::getClassType().name.c_str();
		}

		/* ATM makes use of RTTI. Swap typeid.name() with name */
		static void print() {
			std::cout << "ComponentTypeInfo: " << T::getClassType().name << "(" << id << ")\n";
			std::cout << "Derived: ";
			auto derivedList = type.getDerivedList();
			for (std::size_t i = 0; i < derivedList.size(); i++) {
				std::cout << derivedList[i];
				if (i != derivedList.size() - 1)
					std::cout << ", ";
			}
			std::cout << "\n";
		}

	private:
		static ComponentTypeID id;
		static ComponentType type;
	};

	template<typename T>
	ComponentTypeID ComponentTypeInfo<T>::id = 0;

	template<typename T>
	ComponentType ComponentTypeInfo<T>::type = ComponentType();
}
#endif