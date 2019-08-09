#ifndef PROPERTY_VALUE_ID_H
#define PROPERTY_VALUE_ID_H
#include <cstddef>
#include <ReflectionParser/ReflectionTypes.h>
namespace Core {
	struct PropertyValueID {
		PropertyValueID(Mirror::Property prop) : prop(prop), arrayIndex(0), isArrayElement(false) {}
		PropertyValueID(Mirror::Property prop, std::size_t arrayIndex) : prop(prop), arrayIndex(arrayIndex), isArrayElement(true) {}
		~PropertyValueID() {}

		bool isArrayElement = false;
		std::size_t arrayIndex = 0;
		Mirror::Property prop;
	};
}
#endif