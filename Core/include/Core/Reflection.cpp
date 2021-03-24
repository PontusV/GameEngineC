#include "Reflection.h"
#include <ReflectionParser/ReflectionTypes.h>
#include "ReflectionPolymorph.generated.h"
#include <cstring>
#include <algorithm>
using namespace Core;

template<typename T>
bool vectorIncludes(const std::vector<T>& vec, T value) {
	for (const T& e : vec) {
		if (e == value) return true;
	}
	return false;
}

InspectorFieldRenderType getRendererFromProp(const Mirror::Property& prop) {
	if (prop.type.isBool()) {
		return InspectorFieldRenderType::BOOL;
	}
	if (prop.type.isDecimal()) {
		return InspectorFieldRenderType::DECIMAL;
	}
	if (prop.type.isSignedNumber()) {
		if (prop.type.isChar())
			return InspectorFieldRenderType::SIGNED_CHAR;
		return InspectorFieldRenderType::SIGNED_NUMBER;
	}
	if (prop.type.isUnsignedNumber()) {
		if (prop.type.isChar())
			return InspectorFieldRenderType::UNSIGNED_CHAR;
		return InspectorFieldRenderType::UNSIGNED_NUMBER;
	}
	if (prop.type.isString()) {
		if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ImagePath"))
			return InspectorFieldRenderType::IMAGE_PATH;
		if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ShaderPath"))
			return InspectorFieldRenderType::SHADER_PATH;
		return InspectorFieldRenderType::STRING;
	}
	if (prop.type.isWideString()) {
		return InspectorFieldRenderType::WIDE_STRING;
	}
	if (prop.type.name == "Core::Vector2") {
		return InspectorFieldRenderType::VECTOR2;
	}
	if (prop.type.name == "Core::Color") {
		return InspectorFieldRenderType::COLOR;
	}
	return InspectorFieldRenderType::NONE;
}

void copyString(const std::string& src, char* dest, std::size_t destSize) {
	std::size_t size = std::min(src.size(), destSize - 1);
	strncpy_s(dest, destSize, src.c_str(), size);
	dest[size] = '\0';
}

ReflectedPropertyData convertPropertyData(const Mirror::Property& prop, std::size_t index, void* instance, std::size_t typeID) {
	ReflectedPropertyData data;
	data.index = index;
	copyString(prop.name, data.name, 256);
	copyString(prop.type.name, data.typeName, 256);
	data.id = 0; // WIP

	data.renderer = getRendererFromProp(prop);

	if (data.renderer == InspectorFieldRenderType::VECTOR2) {
		Vector2* vec = static_cast<Vector2*>(Mirror::getPointer(prop, instance, typeID));
		data.fieldCount = 2;
		data.fieldBuffer[0].dataPtr = &vec->x;
		data.fieldBuffer[1].dataPtr = &vec->y;
	}
	else if (data.renderer == InspectorFieldRenderType::COLOR) {
		Color* color = static_cast<Color*>(Mirror::getPointer(prop, instance, typeID));
		data.fieldCount = 4;
		data.fieldBuffer[0].dataPtr = &color->r;
		data.fieldBuffer[1].dataPtr = &color->g;
		data.fieldBuffer[2].dataPtr = &color->b;
		data.fieldBuffer[3].dataPtr = &color->a;
	}
	else {
		Vector2* vec = static_cast<Vector2*>(Mirror::getPointer(prop, instance, typeID));
		data.fieldCount = 1;
		data.fieldBuffer[0].dataPtr = Mirror::getPointer(prop, instance, typeID);
	}
	return data;
}

void Core::onUpdate(IComponent* component, std::size_t typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	Mirror::onUpdate(component, typeID, type.properties[propIndex]);
}

/*void* Core::getPointer(IComponent* component, std::size_t typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	return Mirror::getPointer(type.properties[propIndex], component, typeID);
}*/

void Core::getTypeName(std::size_t typeID, char* out, std::size_t size) {
	copyString(Mirror::getName(typeID), out, size);
}

std::size_t Core::getPropertiesCount(std::size_t typeID) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.properties.size();
}

void Core::getProperties(std::size_t typeID, void* instance, ReflectedPropertyData* out, std::size_t count) {
	Mirror::Class type = Mirror::getType(typeID);
	std::size_t size = std::min(count, type.properties.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = convertPropertyData(type.properties[i], i, instance, typeID);
	}
}

bool Core::hasAnnotation(std::size_t typeID, const char* annotation) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.hasAnnotation(annotation);
}

std::size_t Core::getDerivedTypeIDsCount(std::size_t typeID) {
	return Mirror::polyGetDerivedTypeIDs(typeID).size();
}

void Core::getDerivedTypeIDs(std::size_t typeID, std::size_t* out, std::size_t count) {
	std::vector<std::size_t> typeIDs = Mirror::polyGetDerivedTypeIDs(typeID);
	std::size_t size = std::min(count, typeIDs.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = typeIDs[i];
	}
}

std::size_t Core::getAllReflectedTypesCount() {
	return Mirror::getAllReflectedTypes().size();
}

void Core::getAllReflectedTypes(ReflectedTypeData* out, std::size_t count) {
	std::vector<Mirror::ReflectedType> types = Mirror::getAllReflectedTypes();
	std::size_t size = std::min(count, types.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i].typeID = types[i].typeID;
		copyString(types[i].typeName, out[i].typeName, 256);
	}
}