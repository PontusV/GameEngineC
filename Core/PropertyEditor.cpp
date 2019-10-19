#include "PropertyEditor.h"
#include "Maths/Vector2.h"
#include "ReflectionPolymorph.generated.h"
using namespace Core;


template<typename T>
void setValue(const PropertyValueID& valueID, ReflectedObjectHandle instanceHandle, Mirror::Property& rootProp, T&& value) {
	void* instance = instanceHandle.get();
	std::size_t typeID = instanceHandle.getTypeID();
	if (valueID.isArrayElement) {
		Mirror::polySetArrayElementValue(valueID.prop, valueID.arrayIndex, instance, typeID, value);
	}
	else {
		Mirror::polySetValue(valueID.prop, instance, typeID, value);
	}
	Mirror::onUpdate(instanceHandle.getRoot(), instanceHandle.getRootPropTypeID(), rootProp);
}

template<typename T>
T getValue(const PropertyValueID& valueID, ReflectedObjectHandle instanceHandle) {
	void* instance = instanceHandle.get();
	std::size_t typeID = instanceHandle.getTypeID();
	if (valueID.isArrayElement) {
		return Mirror::polyGetArrayElementValue<T>(valueID.prop, valueID.arrayIndex, instance, typeID);
	}
	else {
		return Mirror::polyGetValue<T>(valueID.prop, instance, typeID);
	}
}

void PropertyEditor::onTextSubmit(std::wstring value) {
	Mirror::VariableType& type = valueID.prop.type;

	if (type.isNumber()) {
		if (value.empty()) value = L"0";
		if (type.isDecimal()) {
			setValue(valueID, instanceHandle, rootProp, std::stod(value));
		}
		else {
			setValue(valueID, instanceHandle, rootProp, std::stoi(value));
		}
	}
	else
		if (type.isString()) {
			std::string stringValue(value.begin(), value.end());
			setValue(valueID, instanceHandle, rootProp, stringValue);
		}
		else if (type.isWideString()) {
			setValue(valueID, instanceHandle, rootProp, value);
		}
		else if (type.isChar()) {
			// WIP
		}
		else if (type.isEnum()) {
			// WIP
		}
		else {
			throw std::invalid_argument("PropertyEditor::onTextSubmit::ERROR The property is not a valid type(" + type.name + ") for this function.");
		}
}

void PropertyEditor::onBoolSubmit(bool value) {
	setValue(valueID, instanceHandle, rootProp, value);
}

template<typename T>
std::wstring toWString(T value);

// Char
template<>
std::wstring toWString<char>(char value) {
	return std::to_wstring(value);
}

// Number
template<>
std::wstring toWString<int>(int value) {
	return std::to_wstring(value);
}

// Unsigned Number
template<>
std::wstring toWString<unsigned int>(unsigned int value) {
	return std::to_wstring(value);
}

// Decimal Number
template<>
std::wstring toWString<double>(double value) {
	std::wstring result = std::to_wstring(value);
	// Removes trailing 0s
	std::size_t offset = result.at(result.find_last_not_of('0')) == '.' ? 2 : 1;
	result.erase(result.find_last_not_of('0') + offset, std::string::npos);
	return result;
}

// Bool
template<>
std::wstring toWString<bool>(bool value) {
	if (value)
		return L"true";
	else
		return L"false";
}

// String
template<>
std::wstring toWString<std::string>(std::string value) {
	return std::wstring(value.begin(), value.end());
}

// WString
template<>
std::wstring toWString<std::wstring>(std::wstring value) {
	return value;
}

std::wstring PropertyEditor::propertyValueToString(PropertyValueID valueID, ReflectedObjectHandle instanceHandle) {
	Mirror::Property& prop = valueID.prop;
	if (prop.type.isChar()) {
		return toWString(getValue<char>(valueID, instanceHandle));
	}
	else if (prop.type.isNumber()) {
		if (prop.type.isDecimal()) {
			return toWString(getValue<double>(valueID, instanceHandle));
		}
		else if (prop.type.isSignedNumber())
			return toWString(getValue<int>(valueID, instanceHandle));
		else if (prop.type.isUnsignedNumber())
			return toWString(getValue<unsigned int>(valueID, instanceHandle));
	}
	else if (prop.type.isBool()) {
		return toWString(getValue<bool>(valueID, instanceHandle));
	}
	else if (prop.type.isString()) {
		return toWString(getValue<std::string>(valueID, instanceHandle));
	}
	else if (prop.type.isWideString()) {
		return getValue<std::wstring>(valueID, instanceHandle);
	}
	else if (prop.type.isObject()) {
		if (prop.type.name == "Vector2") {
			Vector2 size = getValue<Vector2>(valueID, instanceHandle);
			return L"x: " + std::to_wstring(size.x) + L", y: " + std::to_wstring(size.y);
		}
		else {
			return L"instance of " + std::wstring(prop.type.name.begin(), prop.type.name.end());
		}
	}
	return L"ERROR!";
}