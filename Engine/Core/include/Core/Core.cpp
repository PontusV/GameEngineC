#include "Core.h"
#include "entity/Entity.h"
#include "entity/EntityManager.h"
#include "entity/EntitySerializer.h"
#include "entity/handle/Handle.h"
#include "engine/Engine.h"
#include "engine/ResourceManager.h"
#include "engine/Serializer.h"
#include "prefab/PrefabManager.h"
#include "utils/GUID.h"
#include "components/graphics/Image.h"
#include "components/graphics/SpriteRenderer.h"
#include "graphics/data/Color.h"
#include "graphics/font/Font.h"
#include "components/RectTransform.h"
#include "components/entity/ParentEntity.h"
#include "components/entity/ChildManager.h"
#include "components/entity/DebugNameComponent.h"
#include "ReflectionPolymorph.generated.h"

#include <algorithm>
#include <cassert>
#include <type_traits>
#include <vector>
#include <string>
#include <ostream>
#include <streambuf>
#include "maths/MatrixTransform.h"
#include "maths/Vector2.h"
#include "utils/string.h"
using namespace Core;

constexpr const char* TRANSFORM_POSITION_PROPERTY_NAME = "position";

enum class PropertyFieldValueType {
	NONE = 0,
	ARITHMETIC_TYPE,
	STRING,
	WIDE_STRING
};

enum class InspectorFieldType {
	NONE = 0,
	BOOL = 1,
	DECIMAL = 2,
	SIGNED_NUMBER = 3,
	UNSIGNED_NUMBER = 4,
	STRING = 5,
	WIDE_STRING = 6,
	IMAGE_PATH = 7,
	SHADER_PATH = 8,
	FONT_PATH = 9
};

struct PropertyField {
	void* ptr;
	void* valuePtr;
	std::string name;
	std::size_t typeSize;
	PropertyFieldValueType valueType;
	InspectorFieldType fieldType;
};

/* Contains all fields for the property that may be viewed and edited by an external editor */
struct PropertyFieldsData {
	std::size_t iterationCount;
	std::string typeName;
	std::vector<PropertyField> fields;
};

template <typename char_type>
struct ostreambuf : public std::basic_streambuf<char_type, std::char_traits<char_type> > {
public:
	ostreambuf(char_type* buffer, std::streamsize bufferLength) {
		// set the "put" pointer the start of the buffer and record it's length.
		this->setp(buffer, buffer + bufferLength);
	}
protected:
	std::streampos seekoff(std::streamoff off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::out) override {
		if (which != std::ios_base::out)
			throw std::invalid_argument("ostreambuf::seekoff::ERROR Not supported. Only supports out");

		if (dir == std::ios_base::cur)
			this->pbump(off);
		else if (dir == std::ios_base::end)
			this->setp(this->pbase(), this->epptr() + off, this->epptr());
		else if (dir == std::ios_base::beg)
			this->setp(this->pbase(), this->pbase() + off, this->epptr());
		return this->pptr() - this->pbase();
	}

	std::streampos seekpos(std::streampos pos, std::ios_base::openmode which) override {
		return seekoff(pos - std::streampos(std::streamoff(0)), std::ios_base::beg, which);
	}
};

template <typename char_type>
struct istreambuf : public std::basic_streambuf<char_type, std::char_traits<char_type>> {
public:
	istreambuf(char_type* buffer, std::streamsize bufferLength) {
		// Set the "get" pointer to the start of the buffer, the next item, and record its length.
		this->setg(buffer, buffer, buffer + bufferLength);
	}
protected:
	std::streampos seekoff(std::streamoff off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) {
		if (which != std::ios_base::in)
			throw std::invalid_argument("istreambuf::seekoff::ERROR Not supported. Only supports in");

		if (dir == std::ios_base::cur)
			this->gbump(off);
		else if (dir == std::ios_base::end)
			this->setg(this->eback(), this->egptr() + off, this->egptr());
		else if (dir == std::ios_base::beg)
			this->setg(this->eback(), this->eback() + off, this->egptr());
		return this->gptr() - this->eback();
	}

	std::streampos seekpos(std::streampos pos, std::ios_base::openmode which) override {
		return seekoff(pos - std::streampos(std::streamoff(0)), std::ios_base::beg, which);
	}
};

// Utils
template<typename T>
bool vectorIncludes(const std::vector<T>& vec, T value) {
	for (const T& e : vec) {
		if (e == value) return true;
	}
	return false;
}

void copyString(const std::string& src, char* dest, std::size_t destSize) {
	std::size_t size = std::min(src.size(), destSize - 1);
	strncpy_s(dest, destSize, src.c_str(), size);
	dest[size] = '\0';
}

template<typename T>
InspectorFieldType getFieldType() {
	if (std::is_same<T, bool>::value) {
		return InspectorFieldType::BOOL;
	}
	if (std::is_integral<T>::value) {
		if (std::is_signed<T>::value) {
			return InspectorFieldType::SIGNED_NUMBER;
		}
		else {
			return InspectorFieldType::UNSIGNED_NUMBER;
		}
	}
	if (std::is_floating_point<T>::value) {
		return InspectorFieldType::DECIMAL;
	}
	if (std::is_same<T, std::string>::value) {
		return InspectorFieldType::STRING;
	}
	if (std::is_same<T, std::wstring>::value) {
		return InspectorFieldType::WIDE_STRING;
	}
	return InspectorFieldType::NONE;
}

InspectorFieldType getFieldTypeFromProp(const Mirror::Property& prop, const Mirror::VariableType& type) {
	if (type.name == "bool") {
		return InspectorFieldType::BOOL;
	}
	if (type.isIntegral) {
		if (type.isSigned) {
			return InspectorFieldType::SIGNED_NUMBER;
		}
		else {
			return InspectorFieldType::UNSIGNED_NUMBER;
		}
	}
	if (type.isFloatingPoint) {
		return InspectorFieldType::DECIMAL;
	}
	if (type.name == "std::string") {
		if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ImagePath"))
			return InspectorFieldType::IMAGE_PATH;
		if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ShaderPath"))
			return InspectorFieldType::SHADER_PATH;
		return InspectorFieldType::STRING;
	}
	if (type.name == "std::wstring") {
		return InspectorFieldType::WIDE_STRING;
	}
	return InspectorFieldType::NONE;
}

PropertyFieldValueType getFieldValueTypeFromVariableType(const Mirror::VariableType& type) {
	if (type.isArithmetic) {
		return PropertyFieldValueType::ARITHMETIC_TYPE;
	}
	if (type.name == "std::string") {
		return PropertyFieldValueType::STRING;
	}
	if (type.name == "std::wstring") {
		return PropertyFieldValueType::WIDE_STRING;
	}
	return PropertyFieldValueType::NONE;
}

template<typename T>
PropertyFieldValueType getFieldValueType() {
	return std::is_arithmetic<T>::value ? PropertyFieldValueType::ARITHMETIC_TYPE : PropertyFieldValueType::NONE;
}

template<>
PropertyFieldValueType getFieldValueType<std::string>() {
	return PropertyFieldValueType::STRING;
}

template<>
PropertyFieldValueType getFieldValueType<std::wstring>() {
	return PropertyFieldValueType::WIDE_STRING;
}

template<typename T>
struct FieldValue {
	FieldValue(std::string name, T& value) : name(name), value(value), fieldType(getFieldType<T>()) {}
	FieldValue(std::string name, T& value, InspectorFieldType fieldType) : name(name), value(value), fieldType(fieldType) {}
	std::string name;
	T& value;
	InspectorFieldType fieldType;
};

template<std::size_t I>
void createFieldsRecursive(std::vector<PropertyField>& data) {} // End of recursion

template<std::size_t I, typename T, typename... Ts>
void createFieldsRecursive(std::vector<PropertyField>& data, FieldValue<T>& arg, FieldValue<Ts>&... args) {
	data[I].name = arg.name;
	data[I].valuePtr = &arg.value;
	data[I].ptr = &arg.value;
	data[I].typeSize = sizeof(arg.value);
	data[I].valueType = getFieldValueType<T>();
	data[I].fieldType = arg.fieldType;
	createFieldsRecursive<I + 1>(data, args...);
}

template<std::size_t I, typename... Ts>
void createFieldsRecursive(std::vector<PropertyField>& data, FieldValue<std::string>& arg, FieldValue<Ts>&... args) {
	data[I].name = arg.name;
	data[I].valuePtr = (void*)arg.value.data();
	data[I].ptr = &arg.value;
	data[I].typeSize = arg.value.size();
	data[I].valueType = getFieldValueType<std::string>();
	data[I].fieldType = arg.fieldType;
	createFieldsRecursive<I + 1>(data, args...);
}

template<std::size_t I, typename... Ts>
void createFieldsRecursive(std::vector<PropertyField>& data, FieldValue<std::wstring>& arg, FieldValue<Ts>&... args) {
	data[I].name = arg.name;
	data[I].valuePtr = (void*)arg.value.data();
	data[I].ptr = &arg.value;
	data[I].typeSize = arg.value.size() * sizeof(wchar_t);
	data[I].valueType = getFieldValueType<std::wstring>();
	data[I].fieldType = arg.fieldType;
	createFieldsRecursive<I + 1>(data, args...);
}

template<typename... Ts>
std::vector<PropertyField> createFields(FieldValue<Ts>... args) {
	std::vector<PropertyField> data(sizeof...(args));
	createFieldsRecursive<0>(data, args...);
	return data;
}

std::vector<PropertyField> getFields(std::string propName, const Mirror::VariableType& type, void* ptr, InspectorFieldType fieldType) {
	if (type.name == "Core::Vector2") {
		Vector2* vec = static_cast<Vector2*>(ptr);
		return createFields(
			FieldValue("x", vec->x),
			FieldValue("y", vec->y)
		);
	}
	else if (type.name == "Core::Color") {
		Color* color = static_cast<Color*>(ptr);
		return createFields(
			FieldValue("r", color->r),
			FieldValue("g", color->g),
			FieldValue("b", color->b),
			FieldValue("a", color->a)
		);
	}
	else if (type.name == "Core::Font") {
		Font* font = static_cast<Font*>(ptr);
		return createFields(
			FieldValue("File_path", font->fileName, InspectorFieldType::FONT_PATH),
			FieldValue("Font_size", font->size)
		);
	}
	else if (type.name == "std::string") {
		std::string* string = static_cast<std::string*>(ptr);
		return createFields(
			FieldValue(propName, *string, fieldType)
		);
	}
	else if (type.name == "std::wstring") {
		std::wstring* wstring = static_cast<std::wstring*>(ptr);
		return createFields(
			FieldValue(propName, *wstring, fieldType)
		);
	}
	else {
		PropertyField field;
		field.name = propName;
		field.ptr = ptr;
		field.valuePtr = ptr;
		field.typeSize = type.size;
		field.valueType = getFieldValueTypeFromVariableType(type);
		field.fieldType = fieldType;
		std::vector<PropertyField> fields;
		fields.push_back(field);
		return fields;
	}
}

PropertyFieldsData getFieldsFromProp(const Mirror::Property& prop, void* instance, TypeID typeID) {
	auto fieldType = getFieldTypeFromProp(prop, prop.type);
	PropertyFieldsData data;
	data.typeName = prop.type.name;

	if (prop.type.isArray) {
		auto elementPtrs = Mirror::getArrayElementPointers(prop, instance, typeID);
		data.iterationCount = elementPtrs.size();
		for (void* ptr : elementPtrs) {
			auto fields = getFields(prop.name, prop.type, ptr, fieldType);
			data.fields.insert(data.fields.begin(), fields.begin(), fields.end());
		}
	}
	else if (prop.type.name == "std::vector") {
		assert(prop.type.templateParams.size() == 1);
		auto templateType = prop.type.templateParams[0];
		auto elementPtrs = Mirror::getArrayElementPointers(prop, instance, typeID);
		data.iterationCount = elementPtrs.size();
		for (void* ptr : elementPtrs) {
			auto fields = getFields(prop.name, templateType, ptr, getFieldTypeFromProp(prop, templateType));
			data.fields.insert(data.fields.begin(), fields.begin(), fields.end());
		}
	}
	else {
		auto propertyPointer = Mirror::getPointer(prop, instance, typeID);
		data.iterationCount = 1;
		data.fields = getFields(prop.name, prop.type, propertyPointer, fieldType);
	}
	return data;
}

std::string getPropertyName(TypeID typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::Property& prop = type.properties[propIndex];
	return prop.name;
}
// End of Utils

Engine* createEngine() {
	return new Engine();
}

void releaseEngine(Engine* ptr) {
	delete ptr;
}

EntityID createTemplateEntity(Core::Engine* engine, const char* name, float x, float y, float width, float height) {
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = entityManager.createEntity(
		DebugNameComponent(name),
		Image("Images/awesomeface.png"),
		RectTransform(x, y, width, height, 0.0f, Alignment::CENTER),
		SpriteRenderer()
	);
	return entity.getID();
}

bool engineInit(Core::Engine* engine, GLADloadproc ptr, int screenWidth, int screenHeight) {
	Window& window = engine->getGraphics().getWindow();
	window.setActive(true);
	window.setResolution(screenWidth, screenHeight);
	window.setBackgroundColor(0.15f, 0.15f, 0.15f);
	window.initGLAD(ptr);

	int result = engine->initiate(true);
	if (result != 0) { // Failed to initiate the engine
		return false;
	}
	return true;
}

void engineTick(Core::Engine* engine, float deltaTime) {
	engine->tick(deltaTime);
}

void engineEditorTick(Core::Engine* engine, float deltaTime) {
	engine->editorTick(deltaTime);
}

void getCameraPosition(Core::Engine* engine, float* out) {
	Camera& camera = engine->getGraphics().getCamera();
	Vector2 vec = camera.getPosition();
	out[0] = vec.x;
	out[1] = vec.y;
}

void setCameraPosition(Core::Engine* engine, float x, float y) {
	Camera& camera = engine->getGraphics().getCamera();
	camera.setPosition(x, y);
}

void setCameraRotation(Core::Engine* engine, float value) {
	Camera& camera = engine->getGraphics().getCamera();
	camera.setRotation(value);
}

void setCameraScale(Core::Engine* engine, float value) {
	Camera& camera = engine->getGraphics().getCamera();
	camera.setScale(value);
}

float getCameraRotation(Core::Engine* engine) {
	Camera& camera = engine->getGraphics().getCamera();
	return camera.getRotation();
}

float getCameraScale(Core::Engine* engine) {
	Camera& camera = engine->getGraphics().getCamera();
	return camera.getScale();
}

void setViewportSize(Core::Engine* engine, float width, float height) {
	engine->resizeViewport(width, height);
}

EntityID createEntity(Core::Engine* engine) {
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = entityManager.createEntity();
	return entity.getID();
}

EntityID getEntityAtPos(Core::Engine* engine, float x, float y) {
	Input& input = engine->getInput();
	Entity entity = input.getEntityAtPos(x, y);
	return entity.getID();
}

void setAssetDirPath(const char* path) {
	return ResourceManager::getInstance().setAssetDirPath(path);
}

bool getRectSize(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Rect rect = rectTransform->getRect();
		out[0] = rect.w;
		out[1] = rect.h;
		return true;
	}
	return false;
}

bool getMinRectScreenPosition(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Camera& camera = engine->getGraphics().getCamera();
	Entity entity = Entity(entityID);

	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Vector2 minPosition = camera.getViewMatrix() * rectTransform->getVertex(0);
		out[0] = minPosition.x;
		out[1] = minPosition.y;
		return true;
	}
	return false;
}

bool getWorldPosition(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Vector2 vec = rectTransform->getPosition();
		out[0] = vec.x;
		out[1] = vec.y;
		return true;
	}
	return false;
}

bool getLocalPosition(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Vector2 vec = rectTransform->getLocalPosition();
		out[0] = vec.x;
		out[1] = vec.y;
		return true;
	}
	return false;
}

bool setLocalPosition(Core::Engine* engine, EntityID entityID, float x, float y) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		rectTransform->setLocalPosition(Vector2(x, y));
		return true;
	}
	return false;
}

bool setWorldPosition(Core::Engine* engine, EntityID entityID, float x, float y) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		rectTransform->setPosition(Vector2(x, y));
		return true;
	}
	return false;
}

bool overridePosition(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	Handle handle(Entity(entityID), &entityManager);
	if (handle.hasComponent<RectTransform>()) {
		std::size_t constexpr typeID = RectTransform::getClassTypeID();
		return prefabManager.overrideProperty(Handle(Entity(entityID), &entityManager), typeID, TRANSFORM_POSITION_PROPERTY_NAME);
	}
	return false;
}

bool removePositionOverride(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	Handle handle(Entity(entityID), &entityManager);
	if (handle.hasComponent<RectTransform>()) {
		std::size_t constexpr typeID = RectTransform::getClassTypeID();
		return prefabManager.removePropertyOverride(handle, typeID, TRANSFORM_POSITION_PROPERTY_NAME);
	}
	return false;
}

bool isPositionOverriden(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	std::size_t constexpr typeID = RectTransform::getClassTypeID();
	auto propertyOverrides = prefabManager.getPropertyOverrides(Handle(Entity(entityID), &entityManager), typeID);
	auto type = Mirror::getType(typeID);
	for (const auto& propertyOverride : propertyOverrides) {
		auto it = std::find_if(type.properties.begin(), type.properties.end(), [&](const Mirror::Property& prop) { return prop.name == TRANSFORM_POSITION_PROPERTY_NAME; });
		if (it != type.properties.end()) {
			return true;
		}
	}
	return false;
}

bool hasAnnotation(TypeID typeID, const char* annotation) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.hasAnnotation(annotation);
}

void getTypeName(TypeID typeID, char* out, std::size_t outSize) {
	copyString(Mirror::getName(typeID), out, outSize);
}

void onUpdate(void* instance, TypeID typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::onUpdate(instance, typeID, type.properties[propIndex]);
}

void getDerivedTypeIDs(TypeID typeID, TypeID* out, std::size_t outSize) {
	std::vector<std::size_t> typeIDs = Mirror::polyGetDerivedTypeIDs(typeID);
	std::size_t size = std::min(outSize, typeIDs.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = typeIDs[i];
	}
}

void getAllReflectedTypes(TypeID* out, std::size_t outSize) {
	std::vector<Mirror::ReflectedType> types = Mirror::getAllReflectedTypes();
	std::size_t size = std::min(outSize, types.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = types[i].typeID;
	}
}

TypeID getTypeIDFromName(const char* name) {
	if (name == nullptr) return 0; // Invalid ID
	return Mirror::getTypeID(name);
}

TypeID getPrefabComponentTypeID() {
	return PrefabComponent::getClassTypeID();
}

std::size_t getDerivedTypeIDsCount(TypeID typeID) {
	return Mirror::polyGetDerivedTypeIDs(typeID).size();
}

std::size_t getAllReflectedTypesCount() {
	return Mirror::getAllReflectedTypes().size();
}

std::size_t getPropertiesCount(TypeID typeID) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.properties.size();
}

void getPropertyInspectorData(TypeID typeID, std::size_t propIndex, void* instance, char* bufferPtr, std::size_t bufferSize) {
	assert(bufferSize > 0);
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	// Serializes property fields
	ostreambuf sbuf(bufferPtr, bufferSize);
	std::ostream os(&sbuf);
	if (os.fail()) {
		std::cout << "DLL::getPropertyInspectorData::ERROR Failed to setup write to buffer" << std::endl;
		bufferPtr[0] = '\0';
		return;
	}
	auto prop = type.properties[propIndex];
	auto fieldData = getFieldsFromProp(prop, instance, typeID);
	os << prop.name << std::endl;
	os << fieldData.typeName << std::endl;
	os << fieldData.iterationCount << std::endl;
	os << fieldData.fields.size() << std::endl;
	for (const auto& field : fieldData.fields) {
		os << field.name << std::endl;
		os << field.valuePtr << std::endl;
		os << (int)field.fieldType << std::endl;
		os << field.typeSize << std::endl;
	}
	if (os.fail()) {
		std::cout << "DLL::getPropertyInspectorData::ERROR Failed to write to buffer. Given buffer size may be too small" << std::endl;
		bufferPtr[0] = '\0';
		return;
	}
}

void setPropertyFieldValue(TypeID typeID, std::size_t propIndex, void* instance, std::size_t fieldIndex, char* buffer, std::size_t bufferSize) {
	auto type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	auto prop = type.properties[propIndex];
	auto fieldData = getFieldsFromProp(prop, instance, typeID);
	assert(fieldIndex < fieldData.fields.size());
	auto field = fieldData.fields[fieldIndex];
	if (field.valueType == PropertyFieldValueType::ARITHMETIC_TYPE) {
		std::memset(field.ptr, 0, field.typeSize);
		std::memcpy(field.ptr, buffer, std::min(field.typeSize, bufferSize));
	}
	else if (field.valueType == PropertyFieldValueType::STRING) {
		std::string value(buffer);
		std::string* dest = static_cast<std::string*>(field.ptr);
		*dest = value;
	}
	else if (field.valueType == PropertyFieldValueType::WIDE_STRING) {
		std::wstring value(reinterpret_cast<wchar_t*>(buffer));
		std::wstring* dest = static_cast<std::wstring*>(field.ptr);
		*dest = value;
	}
}

std::size_t getPropertyIndex(TypeID typeID, const char* propertyName) {
	auto type = Mirror::getType(typeID);
	for (std::size_t i = 0; i < type.properties.size(); i++) {
		if (type.properties[i].name.compare(propertyName) == 0) {
			return i;
		}
	}
	return -1;
}

void getPropertyName(TypeID typeID, std::size_t propIndex, char* buffer, std::size_t bufferSize) {
	std::string name = getPropertyName(typeID, propIndex);
	copyString(name, buffer, bufferSize);
}

bool unpackPrefab(Core::Engine* engine, EntityID entityID) {
	PrefabManager& prefabManager = engine->getPrefabManager();
	EntityManager& entityManager = engine->getEntityManager();
	Handle prefabHandle = Handle(Entity(entityID), &entityManager);
	return prefabManager.unpackPrefab(prefabHandle);
}

bool savePrefab(Core::Engine* engine, EntityID entityID, const char* path) {
	PrefabManager& prefabManager = engine->getPrefabManager();
	EntityManager& entityManager = engine->getEntityManager();
	Handle prefabHandle = Handle(Entity(entityID), &entityManager);
	EntityRemapSaveInfo entityRemapInfo;
	if (PrefabComponent* prefabComponent = prefabHandle.getComponent<PrefabComponent>()) {
		auto connectedEntities = prefabComponent->getConnectedEntities();
		std::for_each(connectedEntities.begin(), connectedEntities.end(),
			[&entityRemapInfo](const ConnectedEntity& e) {
				entityRemapInfo.push_back(std::make_pair(e.entity.getID(), e.guid));
			});
	}
	try {
		return prefabManager.savePrefab(path, prefabHandle, entityRemapInfo);
	}
	catch (std::exception& e) {
		std::cout << "savePrefab::ERROR Failed to save prefab. Error: " << e.what() << std::endl;
		return false;
	}
	catch (...) {
		std::cout << "savePrefab::ERROR Failed to save prefab" << std::endl;
		return false;
	}
}

bool updatePrefabs(Core::Engine* engine, const char* path) {
	PrefabManager& prefabManager = engine->getPrefabManager();
	return prefabManager.updatePrefab(path);
}

bool updatePrefab(Core::Engine* engine, EntityID entityID) {
	PrefabManager& prefabManager = engine->getPrefabManager();
	EntityManager& entityManager = engine->getEntityManager();
	Handle prefabHandle = Handle(Entity(entityID), &entityManager);
	return prefabManager.updatePrefab(prefabHandle);
}

bool createPrefabFromEntity(Core::Engine* engine, EntityID entityID, const char* path) {
	PrefabManager& prefabManager = engine->getPrefabManager();
	EntityManager& entityManager = engine->getEntityManager();
	Handle entityHandle = Handle(Entity(entityID), &entityManager);
	return prefabManager.createPrefabFromEntity(entityHandle, path);
}

EntityID createPrefabEntity(Core::Engine* engine, const char* path, float x, float y) {
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = entityManager.createEntity(
		DebugNameComponent("TEMP"),
		Transform(x, y),
		PrefabComponent(path)
	);
	return entity.getID();
}

EntityID createEntityFromPrefab(Core::Engine* engine, const char* path, float x, float y) {
	PrefabManager& prefabManager = engine->getPrefabManager();
	Handle prefabHandle = prefabManager.createEntityFromPrefab(path, x, y);
	return prefabHandle.getEntity().getID();
}

bool overrideProperty(Core::Engine* engine, EntityID entityID, std::size_t typeID, std::size_t propIndex) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	std::string propertyName = getPropertyName(typeID, propIndex);
	return prefabManager.overrideProperty(Handle(Entity(entityID), &entityManager), typeID, propertyName);
}

bool removePropertyOverride(Core::Engine* engine, EntityID entityID, std::size_t typeID, std::size_t propIndex) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	std::string propertyName = getPropertyName(typeID, propIndex);
	return prefabManager.removePropertyOverride(Handle(Entity(entityID), &entityManager), typeID, propertyName);
}

bool isEntityPrefabRoot(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	return prefabManager.isEntityPrefabRoot(Handle(Entity(entityID), &entityManager));
}

bool isEntityAnOverride(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	return prefabManager.isEntityAnOverride(Handle(Entity(entityID), &entityManager));
}

void getPrefabFilePath(Core::Engine* engine, EntityID entityID, char* out, std::size_t outSize) {
	if (entityID == Entity::INVALID_ID) {
		out[0] = '\0';
		return;
	}
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (PrefabComponent* component = entityManager.getComponent<PrefabComponent>(entity)) {
		copyString(component->getFilePath(), out, outSize);
	}
	else {
		out[0] = '\0';
	}
}

void getPropertyOverrides(Core::Engine* engine, EntityID entityID, TypeID typeID, std::size_t* out, std::size_t outSize) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	auto propertyOverrides = prefabManager.getPropertyOverrides(Handle(Entity(entityID), &entityManager), typeID);
	std::size_t size = std::min(propertyOverrides.size(), outSize);
	auto type = Mirror::getType(typeID);
	for (std::size_t i = 0; i < size; i++) {
		std::string propertyName = propertyOverrides[i].targetPropertyName;
		auto it = std::find_if(type.properties.begin(), type.properties.end(), [&](const Mirror::Property& prop) { return prop.name == propertyName; });
		out[i] = it == type.properties.end() ? -1 : it - type.properties.begin();
	}
}

void getPropertyOverridesAt(Core::Engine* engine, EntityID entityID, EntityID* entityIDOut, TypeID* typeIDOut, std::size_t* propIndexOut, std::size_t outSize) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	auto propertyOverrides = prefabManager.getPropertyOverridesAt(Handle(Entity(entityID), &entityManager));
	std::size_t size = std::min(propertyOverrides.size(), outSize);
	for (std::size_t i = 0; i < size; i++) {
		std::size_t typeID = propertyOverrides[i].targetComponent.getComponentTypeID();
		entityIDOut[i] = propertyOverrides[i].targetComponent.getOwner().getEntity().getID();
		typeIDOut[i] = typeID;
		propIndexOut[i] = getPropertyIndex(typeID, propertyOverrides[i].targetPropertyName.c_str());
	}
}

void getComponentOverrides(Core::Engine* engine, EntityID entityID, TypeID* out, std::size_t outSize) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	auto componentOverrides = prefabManager.getComponentOverrides(Handle(Entity(entityID), &entityManager));
	std::size_t size = std::min(componentOverrides.size(), outSize);
	for (std::size_t i = 0; i < size; i++) {
		out[i] = componentOverrides[i];
	}
}

bool clearPropertyOverrides(Core::Engine* engine, EntityID entityID, TypeID typeID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	Handle handle(Entity(entityID), &entityManager);
	for (PrefabComponent* prefabComponent : handle.getComponentsInParents<PrefabComponent>()) {
		prefabComponent->clearPropertyOverrides(Entity(entityID), typeID);
		return true;
	}
	return false;
}

bool revertPrefab(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	prefabManager.revertPrefab(Handle(Entity(entityID), &entityManager));
	return true;
}

EntityID getPrefabOverrideReceiver(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	return prefabManager.getPrefabOverrideReceiver(Handle(Entity(entityID), &entityManager)).getEntity().getID();
}

std::size_t getComponentOverridesCount(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	return prefabManager.getComponentOverrides(Handle(Entity(entityID), &entityManager)).size();
}

std::size_t getPropertyOverridesCount(Core::Engine* engine, EntityID entityID, TypeID typeID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	return prefabManager.getPropertyOverrides(Handle(Entity(entityID), &entityManager), typeID).size();
}

std::size_t getPropertyOverridesAtCount(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	PrefabManager& prefabManager = engine->getPrefabManager();
	return prefabManager.getPropertyOverridesAt(Handle(Entity(entityID), &entityManager)).size();
}

bool loadPropertyFromBuffer(Core::Engine* engine, EntityID entityID, TypeID typeID, char* serializedData, std::size_t dataSize) {
	EntityManager& entityManager = engine->getEntityManager();
	istreambuf sbuf(serializedData, dataSize);
	std::istream is(&sbuf);
	DeserializerArchive ar(is, &entityManager);
	std::string propertyName;
	ar(propertyName);
	Entity entity(entityID);
	auto instance = entityManager.getComponent(entity, typeID);
	if (instance == nullptr) {
		return false;
	}
	std::cout << "Loading property entityID " << entityID << ", typeID " << typeID << ", propertyName " << propertyName << std::endl;
	return Mirror::deserializeProperty(ar, instance, typeID, propertyName);
}

bool loadComponentFromBuffer(Core::Engine* engine, EntityID entityID, char* serializedData, std::size_t dataSize) {
	EntityManager& entityManager = engine->getEntityManager();
	istreambuf sbuf(serializedData, dataSize);
	std::istream is(&sbuf);
	DeserializerArchive ar(is, &entityManager);
	std::string typeName;
	ar(typeName);

	std::size_t typeID = Mirror::getTypeID(typeName);
	Handle handle = Handle(Entity(entityID), &entityManager);
	if (handle.hasComponent(typeID)) {
		handle.removeComponent(typeID);
	}
	EntityHandle entityHandle = handle;
	ComponentLoader::addComponentFromFile(ar, typeName, entityHandle);
	return true;
}

bool loadEntityFromBuffer(Core::Engine* engine, char* serializedData, std::size_t dataSize) {
	EntityManager& entityManager = engine->getEntityManager();
	istreambuf sbuf(serializedData, dataSize);
	std::istream is(&sbuf);
	DeserializerArchive archive(is, &entityManager);
	std::size_t entityAmount;
	archive(entityAmount);

	std::vector<Entity> entities;
	entities.reserve(entityAmount);
	for (std::size_t i = 0; i < entityAmount; i++) {
		std::size_t entityID;
		archive(entityID);

		Entity entity = Entity(entityID);
		auto location = entityManager.getLocationDetailed(entity);
		if (location.isValid()) {
			std::cout << "loadEntityFromBuffer::WARNING An entity with loaded EntityID already exists. Destroying the entity..." << std::endl;
			entityManager.destroyEntity(location);
		}
		entityManager.addEntity(entity);
		entities.push_back(entity);
	}
	for (const Entity& entity : entities) {
		Handle handle(entity, &entityManager);
		deserializeEntity(handle, archive);
	}
	return true;
}

std::size_t writePropertyToBuffer(Core::Engine* engine, EntityID entityID, TypeID typeID, std::size_t propIndex, char* bufferPtr, std::size_t bufferSize) {
	EntityManager& entityManager = engine->getEntityManager();
	auto type = Mirror::getType(typeID);
	assert(type.properties.size() > propIndex);
	auto prop = type.properties[propIndex];
	
	ostreambuf sbuf(bufferPtr, bufferSize);
	std::ostream os(&sbuf);
	SerializerArchive ar(os);
	ar(prop.name);

	Entity entity(entityID);
	auto instance = entityManager.getComponent(entity, typeID);
	if (instance == nullptr) {
		return 0;
	}
	if (Mirror::serializeProperty(ar, instance, typeID, prop.name)) {
		return os.tellp();
	}
	return 0;
}

std::size_t writeComponentToBuffer(Core::Engine* engine, EntityID entityID, TypeID typeID, char* bufferPtr, std::size_t bufferSize) {
	EntityManager& entityManager = engine->getEntityManager();
	auto type = Mirror::getType(typeID);

	ostreambuf sbuf(bufferPtr, bufferSize);
	std::ostream os(&sbuf);
	SerializerArchive ar(os);
	ar(type.name);

	Entity entity(entityID);
	if (IComponentData* component = entityManager.getComponent(entity, typeID)) {
		Mirror::serialize(component, typeID, ar);
		return os.tellp();
	}
	return 0;
}

std::size_t writeEntityToBuffer(Core::Engine* engine, EntityID entityID, char* bufferPtr, std::size_t bufferSize) {
	EntityManager& entityManager = engine->getEntityManager();
	ostreambuf sbuf(bufferPtr, bufferSize);
	std::ostream os(&sbuf);
	SerializerArchive archive(os);

	std::vector<Handle> entities;
	Handle handle(Entity(entityID), &entityManager);
	entities.push_back(handle);
	std::size_t childCount = handle.getChildCount();
	entities.reserve(childCount + 1);
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = handle.getChild(i);
		entities.push_back(child);
	}

	std::size_t entityAmount = entities.size();
	archive(entityAmount);

	for (const Handle& entity : entities) {
		std::size_t entityID = entity.getEntity().getID();
		archive(entityID);
	}

	for (const Handle& entity : entities) {
		serializeEntity(entity, archive);
	}
	return os.tellp();
}

bool loadGameState(Core::Engine* engine, const char* path) {
	EntityManager& entityManager = engine->getEntityManager();
	std::ifstream file;
	try {
		file.open(path, std::ios::in | std::ios::binary);
	}
	catch (std::system_error&) {
		std::cout << "loadGameState::ERROR Failed to load file: " << path << ". File does not exist" << std::endl;
		return false;
	}
	if (file.fail()) {
		std::cout << "loadGameState::ERROR Failed to load file: " << path << ". File open failed" << std::endl;
		return false;
	}
	DeserializerArchive archive(file, &entityManager);
	std::size_t entityIDCounter;
	archive(entityIDCounter);

	entityManager.clear();
	entityManager.setEntityIDCounter(entityIDCounter);
	std::size_t entityAmount;
	archive(entityAmount);

	std::vector<Entity> entities;
	entities.reserve(entityAmount);
	for (std::size_t i = 0; i < entityAmount; i++) {
		std::size_t entityID;
		archive(entityID);

		Entity entity = Entity(entityID);
		entityManager.addEntity(entity);
		entities.push_back(entity);
	}
	/*EntityRemapLoadInfo entityRemapInfo;
	for (const Entity& entity : entities) {
		std::string entityGUID;
		archive(entityGUID);
		entityRemapInfo.insert(std::pair<std::string, std::size_t>(entityGUID, entity.getID()));
	}
	archive.setEntityRemapInfo(entityRemapInfo);*/
	for (const Entity& entity : entities) {
		Handle handle(entity, &entityManager);
		deserializeEntity(handle, archive);
	}
	file.close();
	return true;
}

bool saveGameState(Core::Engine* engine, const char* path) {
	EntityManager& entityManager = engine->getEntityManager();
	std::ofstream file;
	try {
		file.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
	}
	catch (std::system_error&) {
		std::cout << "loadGameState::ERROR Failed to load file: " << path << ". File does not exist" << std::endl;
		return false;
	}
	if (file.fail()) {
		std::cout << "loadGameState::ERROR Failed to load file: " << path << ". File open failed" << std::endl;
		return false;
	}
	SerializerArchive archive(file);
	std::size_t entityIDCounter = entityManager.getEntityIDCounter();
	archive(entityIDCounter);

	auto entities = entityManager.getAllEntities();
	std::size_t entityAmount = entities.size();
	archive(entityAmount);

	for (auto& entity : entities) {
		std::size_t entityID = entity.getID();
		archive(entityID);
	}
	/*EntityRemapSaveInfo entityRemapInfo;
	for (const auto& entity : entities) {
		std::string entityGUID = Core::GUID::create(); // TODO: Make sure it is unique
		archive(entityGUID);
		entityRemapInfo.insert(std::pair<std::size_t, std::string>(entity.getID(), entityGUID));
	}
	archive.setEntityRemapInfo(entityRemapInfo);*/
	for (auto& entity : entities) {
		Handle handle(entity, &entityManager);
		serializeEntity(handle, archive);
	}
	file.close();
	return true;
}

bool destroyEntity(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	return entityManager.destroyEntity(entity);
}

bool getAllEntities(Core::Engine* engine, EntityID* out, std::size_t outSize) {
	EntityManager& entityManager = engine->getEntityManager();
	std::vector<Entity> entities = entityManager.getAllEntities();
	std::size_t size = std::min(outSize, entities.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = entities[i].getID();
	}
	return true;
}

std::size_t getAllEntitiesCount(Core::Engine* engine) {
	EntityManager& entityManager = engine->getEntityManager();
	std::vector<Entity> entities = entityManager.getAllEntities();
	return entities.size();
}

void setCallbackPtr(Core::Engine* engine, void* ptr) {
	EntityManager& entityManager = engine->getEntityManager();
	entityManager.setCallbackPtr(ptr);
}

void setEntitiesChangedCallback(Core::Engine* engine, EntitiesChangedCallbackFun callback) {
	EntityManager& entityManager = engine->getEntityManager();
	entityManager.setEntitiesChangedCallback(callback);
}

bool isEntityChild(Core::Engine* engine, EntityID entityID, EntityID parentID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	ParentEntity* component = entityManager.getComponent<ParentEntity>(entity);
	if (component == nullptr) return false;
	return parentID == component->getParent().getEntity().getID();
}

bool setEntityParent(Core::Engine* engine, EntityID entityID, EntityID parentID) {
	if (entityID == Entity::INVALID_ID || entityID == parentID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	Entity parentEntity = Entity(parentID);
	Handle handle = Handle(entity, &entityManager);
	if (parentID != Entity::INVALID_ID && handle.isChild(parentEntity)) return false;
	Handle parentHandle = Handle(parentEntity, &entityManager);
	handle.setParent(parentEntity, true);
	return true;
}

bool hasEntityParent(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	return entityManager.hasComponent<ParentEntity>(entity);
}

bool detachEntityParent(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	Handle handle = Handle(entity, &entityManager);
	handle.removeParent(true);
	return true;
}

void getEntityName(Core::Engine* engine, EntityID entityID, char* out, std::size_t outSize) {
	if (entityID == Entity::INVALID_ID) {
		out[0] = '\0';
		return;
	}
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (DebugNameComponent* component = entityManager.getComponent<DebugNameComponent>(entity)) {
		copyString(component->getName(), out, outSize);
	}
	else {
		out[0] = '\0';
	}
}

EntityID getEntityParent(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (ParentEntity* parentEntity = entityManager.getComponent<ParentEntity>(entity)) {
		return parentEntity->getParent().getEntity().getID();
	}
	return Entity::INVALID_ID;
}

EntityID getEntityChild(Core::Engine* engine, EntityID entityID, std::size_t index) {
	if (entityID == Entity::INVALID_ID) return Entity::INVALID_ID;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	Handle handle = Handle(entity, &entityManager);
	return handle.getChild(index).getEntity().getID();
}

std::size_t getEntityChildCount(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return 0;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	Handle handle = Handle(entity, &entityManager);
	return handle.getChildCount();
}

std::size_t getEntityImmediateChildCount(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return 0;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	ChildManager* component = entityManager.getComponent<ChildManager>(entity);
	if (component == nullptr) return 0;
	return component->getChildCount();
}


bool addComponent(Core::Engine* engine, EntityID entityID, TypeID typeID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	EntityLocationDetailed location = entityManager.addComponent(entity, typeID);
	return location.isValid();
}

bool removeComponent(Core::Engine* engine, EntityID entityID, TypeID typeID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	EntityLocationDetailed location = entityManager.removeComponent(entity, typeID);
	return location.isValid();
}

bool hasComponent(Core::Engine* engine, EntityID entityID, TypeID typeID) {
	EntityManager& entityManager = engine->getEntityManager();
	return entityManager.hasComponent(Entity(entityID), typeID);
}

bool setEntityName(Core::Engine* engine, EntityID entityID, const char* name) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (DebugNameComponent* component = entityManager.getComponent<DebugNameComponent>(entity)) {
		component->setName(name);
		return true;
	}
	else {
		DebugNameComponent newComponent(name);
		entityManager.addComponent(entity, newComponent);
	}
	return true;
}

void getComponents(Core::Engine* engine, EntityID entityID, void** outPtrs, TypeID* outTypeIDs, std::size_t outSize) {
	if (entityID == Entity::INVALID_ID) return;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	std::vector<IComponentData*> components = entityManager.getComponents(entity);
	std::size_t size = std::min(components.size(), outSize);
	for (std::size_t i = 0; i < size; i++) {
		outPtrs[i] = components[i];
		outTypeIDs[i] = components[i]->getTypeID();
	}
}

std::size_t getComponentsCount(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return 0;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	std::vector<IComponentData*> components = entityManager.getComponents(entity);
	return components.size();
}