#include "EngineDLLInterface.h"
#include <iostream>
#include <algorithm>
#include <streambuf>
using namespace Editor;

template <typename char_type>
struct ostreambuf : public std::basic_streambuf<char_type, std::char_traits<char_type>> {
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

EnginePtr EngineDLLInterface::createEngine() {
    if (createEngineFun == nullptr) {
        std::cout << "EngineDLLInterface::createEngine::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createEngine::ERROR The function ptr is nullptr";
    }
    if (engine) {
        std::cout << "EngineDLLInterface::createEngine::ERROR Failed to create Engine since one already exists!" << std::endl;
        return engine;
    }
    engine = createEngineFun();
    return engine;
}

void EngineDLLInterface::releaseEngine() {
    if (releaseEngineFun == nullptr) {
        std::cout << "EngineDLLInterface::releaseEngine::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::releaseEngine::ERROR The function ptr is nullptr";
    }
    if (!engine) {
        std::cout << "EngineDLLInterface::releaseEngine::ERROR Failed to release Engine since none currently exists!" << std::endl;
    }
    releaseEngineFun(engine);
    engine = nullptr;
}

EntityID EngineDLLInterface::createTemplateEntity(const char* name, float x, float y, float width, float height) {
    if (createTemplateEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::createTemplateEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createTemplateEntity::ERROR The function ptr is nullptr";
    }
    return createTemplateEntityFun(engine, name, x, y, width, height);
}

bool EngineDLLInterface::engineInit(GLADloadproc ptr, int screenWidth, int screenHeight) {
    if (engineInitFun == nullptr) {
        std::cout << "EngineDLLInterface::engineInit::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::engineInit::ERROR The function ptr is nullptr";
    }
    return engineInitFun(engine, ptr, screenWidth, screenHeight);
}

bool EngineDLLInterface::loadGameState(const char* path) {
    if (loadGameStateFun == nullptr) {
        std::cout << "EngineDLLInterface::loadGameState::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::loadGameState::ERROR The function ptr is nullptr";
    }
    return loadGameStateFun(engine, path);
}

bool EngineDLLInterface::saveGameState(const char* path) {
    if (saveGameStateFun == nullptr) {
        std::cout << "EngineDLLInterface::saveGameState::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::saveGameState::ERROR The function ptr is nullptr";
    }
    return saveGameStateFun(engine, path);
}

void EngineDLLInterface::engineTick(float deltaTime) {
    if (engineTickFun == nullptr) {
        std::cout << "EngineDLLInterface::engineTick::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::engineTick::ERROR The function ptr is nullptr";
    }
    engineTickFun(engine, deltaTime);
}

void EngineDLLInterface::engineEditorTick(float deltaTime) {
    if (engineEditorTickFun == nullptr) {
        std::cout << "EngineDLLInterface::engineEditorTick::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::engineEditorTick::ERROR The function ptr is nullptr";
    }
    engineEditorTickFun(engine, deltaTime);
}

void EngineDLLInterface::setCameraPosition(float x, float y) {
    if (setCameraPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::setCameraPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setCameraPosition::ERROR The function ptr is nullptr";
    }
    setCameraPositionFun(engine, x, y);
}

void EngineDLLInterface::setCameraRotation(float value) {
    if (setCameraRotationFun == nullptr) {
        std::cout << "EngineDLLInterface::setCameraRotation::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setCameraRotation::ERROR The function ptr is nullptr";
    }
    setCameraRotationFun(engine, value);
}

void EngineDLLInterface::setCameraScale(float value) {
    if (setCameraScaleFun == nullptr) {
        std::cout << "EngineDLLInterface::setCameraScale::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setCameraScale::ERROR The function ptr is nullptr";
    }
    setCameraScaleFun(engine, value);
}

ImVec2 EngineDLLInterface::getCameraPosition() {
    if (getCameraPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getCameraPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getCameraPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getCameraPositionFun(engine, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

float EngineDLLInterface::getCameraRotation() {
    if (getCameraRotationFun == nullptr) {
        std::cout << "EngineDLLInterface::getCameraRotation::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getCameraRotation::ERROR The function ptr is nullptr";
    }
    return getCameraRotationFun(engine);
}

float EngineDLLInterface::getCameraScale() {
    if (getCameraScaleFun == nullptr) {
        std::cout << "EngineDLLInterface::getCameraScale::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getCameraScale::ERROR The function ptr is nullptr";
    }
    return getCameraScaleFun(engine);
}

void EngineDLLInterface::setViewportSize(float width, float height) {
    if (setViewportSizeFun == nullptr) {
        std::cout << "EngineDLLInterface::setViewportSize::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setViewportSize::ERROR The function ptr is nullptr";
    }
    setViewportSizeFun(engine, width, height);
}

void EngineDLLInterface::setAssetDirPath(const char* path) {
    if (setAssetDirPathFun == nullptr) {
        std::cout << "EngineDLLInterface::setAssetDirPath::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setAssetDirPath::ERROR The function ptr is nullptr";
    }
    setAssetDirPathFun(path);
}

EntityID EngineDLLInterface::createEntity(const char* name) {
    if (createEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::createEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createEntity::ERROR The function ptr is nullptr";
    }
    return createEntityFun(engine, name);
}

EntityID EngineDLLInterface::getEntityAtPos(float x, float y) {
    if (getEntityAtPosFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityAtPos::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityAtPos::ERROR The function ptr is nullptr";
    }
    return getEntityAtPosFun(engine, x, y);
}

ImVec2 EngineDLLInterface::getRectSize(EntityID entityID) {
    if (getRectSizeFun == nullptr) {
        std::cout << "EngineDLLInterface::getRectSize::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getRectSize::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getRectSizeFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

ImVec2 EngineDLLInterface::getMinRectScreenPosition(EntityID entityID) {
    if (getMinRectScreenPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getMinRectScreenPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getMinRectScreenPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getMinRectScreenPositionFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

ImVec2 EngineDLLInterface::getWorldPosition(EntityID entityID) {
    if (getWorldPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getWorldPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getWorldPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getWorldPositionFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

ImVec2 EngineDLLInterface::getLocalPosition(EntityID entityID) {
    if (getLocalPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getLocalPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getLocalPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getLocalPositionFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

bool EngineDLLInterface::setLocalPosition(EntityID entityID, float x, float y) {
    if (setLocalPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::setLocalPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setLocalPosition::ERROR The function ptr is nullptr";
    }
    return setLocalPositionFun(engine, entityID, x, y);
}

bool EngineDLLInterface::setWorldPosition(EntityID entityID, float x, float y) {
    if (setWorldPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::setWorldPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setWorldPosition::ERROR The function ptr is nullptr";
    }
    return setWorldPositionFun(engine, entityID, x, y);
}

bool EngineDLLInterface::overridePosition(EntityID entityID) {
    if (overridePositionFun == nullptr) {
        std::cout << "EngineDLLInterface::overridePosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::overridePosition::ERROR The function ptr is nullptr";
    }
    return overridePositionFun(engine, entityID);
}

bool EngineDLLInterface::removePositionOverride(EntityID entityID) {
    if (removePositionOverrideFun == nullptr) {
        std::cout << "EngineDLLInterface::removePositionOverride::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::removePositionOverride::ERROR The function ptr is nullptr";
    }
    return removePositionOverrideFun(engine, entityID);
}

bool EngineDLLInterface::isPositionOverriden(EntityID entityID) {
    if (isPositionOverridenFun == nullptr) {
        std::cout << "EngineDLLInterface::isPositionOverriden::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::isPositionOverriden::ERROR The function ptr is nullptr";
    }
    return isPositionOverridenFun(engine, entityID);
}

bool EngineDLLInterface::hasAnnotation(TypeID typeID, const char* annotation) {
    if (hasAnnotationFun == nullptr) {
        std::cout << "EngineDLLInterface::hasAnnotation::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::hasAnnotation::ERROR The function ptr is nullptr";
    }
    return hasAnnotationFun(typeID, annotation);
}

std::string EngineDLLInterface::getTypeName(TypeID typeID) {
    if (getTypeNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getTypeName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getTypeName::ERROR The function ptr is nullptr";
    }
    getTypeNameFun(typeID, readBuffer.data(), sizeof(readBuffer));
    return std::string(readBuffer.data());
}

void EngineDLLInterface::onUpdate(void* instance, TypeID typeID, std::size_t propIndex) {
    if (onUpdateFun == nullptr) {
        std::cout << "EngineDLLInterface::onUpdate::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::onUpdate::ERROR The function ptr is nullptr";
    }
    onUpdateFun(instance, typeID, propIndex);
}

std::vector<TypeID> EngineDLLInterface::getDerivedTypeIDs(TypeID typeID) {
    if (getDerivedTypeIDsFun == nullptr) {
        std::cout << "EngineDLLInterface::getDerivedTypeIDs::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getDerivedTypeIDs::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getDerivedTypeIDsCount(typeID);
    std::vector<TypeID> types(outSize);
    getDerivedTypeIDsFun(typeID, &types[0], outSize);
    return types;
}

std::vector<ReflectedTypeData> EngineDLLInterface::getAllReflectedTypes() {
    if (getAllReflectedTypesFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllReflectedTypes::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllReflectedTypes::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getAllReflectedTypesCount();
    std::vector<TypeID> typeIDs(outSize);
    getAllReflectedTypesFun(&typeIDs[0], outSize);
    std::vector<ReflectedTypeData> types(outSize);
    for (std::size_t i = 0; i < outSize; i++) {
        types[i].typeID = typeIDs[i];
        types[i].typeName = getTypeName(typeIDs[i]);
    }
    return types; // TODO: Cache result. Result may only change between reloads
}

TypeID EngineDLLInterface::getTypeIDFromName(const char* name) {
    if (getTypeIDFromNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getTypeIDFromName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getTypeIDFromName::ERROR The function ptr is nullptr";
    }
    return getTypeIDFromNameFun(name);
}

TypeID EngineDLLInterface::getPrefabComponentTypeID() {
    if (getPrefabComponentTypeIDFun == nullptr) {
        std::cout << "EngineDLLInterface::getPrefabComponentTypeID::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPrefabComponentTypeID::ERROR The function ptr is nullptr";
    }
    return getPrefabComponentTypeIDFun();
}

std::size_t EngineDLLInterface::getDerivedTypeIDsCount(TypeID typeID) {
    if (getDerivedTypeIDsCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getDerivedTypeIDsCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getDerivedTypeIDsCount::ERROR The function ptr is nullptr";
    }
    return getDerivedTypeIDsCountFun(typeID);
}

std::size_t EngineDLLInterface::getAllReflectedTypesCount() {
    if (getAllReflectedTypesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllReflectedTypesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllReflectedTypesCount::ERROR The function ptr is nullptr";
    }
    return getAllReflectedTypesCountFun();
}

std::size_t EngineDLLInterface::getPropertiesCount(TypeID typeID) {
    if (getPropertiesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertiesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertiesCount::ERROR The function ptr is nullptr";
    }
    return getPropertiesCountFun(typeID);
}

std::vector<PropertyData> EngineDLLInterface::getProperties(EntityID entityID, TypeID typeID, void* instance) {
    std::size_t size = getPropertiesCount(typeID);
    std::size_t overrideSize = getPropertyOverridesCount(entityID, typeID);
    std::vector<PropertyData> properties(size);
    std::vector<std::size_t> propertyOverrides(overrideSize);
    getPropertyOverridesFun(engine, entityID, typeID, &propertyOverrides[0], overrideSize);

    for (std::size_t i = 0; i < size; i++) {
        getPropertyInspectorData(typeID, i, instance, readBuffer.data(), sizeof(readBuffer));
        if (readBuffer[0] == '\0') {
            std::cout << "EngineDLLInterface::getProperties::ERROR Failed to read property" << std::endl;
            continue;
        }
        properties[i].index = i;
        properties[i].overriden = std::find(propertyOverrides.begin(), propertyOverrides.end(), i) != propertyOverrides.end();
        // Read serialized data from buffer
        istreambuf sbuf(readBuffer.data(), sizeof(readBuffer));
        std::istream is(&sbuf);
        is >> properties[i].name;
        is >> properties[i].typeName;
        is >> properties[i].arraySize;
        std::size_t fieldCount;
        is >> fieldCount;
        properties[i].fields.resize(fieldCount);
        for (std::size_t fieldIndex = 0; fieldIndex < fieldCount; fieldIndex++) {
            PropertyField& field = properties[i].fields[fieldIndex];
            is >> field.name;
            is >> field.ptr;
            int fieldType;
            is >> fieldType;
            field.type = static_cast<InspectorFieldType>(fieldType);
            is >> field.typeSize;
        }
    }
    return properties;
}

std::size_t EngineDLLInterface::getPropertyIndex(TypeID typeID, const char* propName) {
    if (getPropertyIndexFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyIndex::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyIndex::ERROR The function ptr is nullptr";
    }
    return getPropertyIndexFun(typeID, propName);
}

std::string EngineDLLInterface::getPropertyName(TypeID typeID, std::size_t propIndex) {
    if (getPropertyNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyName::ERROR The function ptr is nullptr";
    }
    getPropertyNameFun(typeID, propIndex, readBuffer.data(), sizeof(readBuffer));
    return std::string(readBuffer.data());
}

void EngineDLLInterface::getPropertyInspectorData(TypeID typeID, std::size_t propIndex, void* instance, char* buffer, std::size_t bufferSize) {
    if (getPropertyInspectorDataFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyInspectorData::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyInspectorData::ERROR The function ptr is nullptr";
    }
    return getPropertyInspectorDataFun(typeID, propIndex, instance, buffer, bufferSize);
}

void EngineDLLInterface::setPropertyFieldValue(TypeID typeID, std::size_t propIndex, void* instance, std::size_t fieldIndex, char* buffer, std::size_t bufferSize) {
    if (setPropertyFieldValueFun == nullptr) {
        std::cout << "EngineDLLInterface::setPropertyFieldValue::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setPropertyFieldValue::ERROR The function ptr is nullptr";
    }
    return setPropertyFieldValueFun(typeID, propIndex, instance, fieldIndex, buffer, bufferSize);
}

bool EngineDLLInterface::unpackPrefab(EntityID entityID) {
    if (unpackPrefabFun == nullptr) {
        std::cout << "EngineDLLInterface::unpackPrefab::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::unpackPrefab::ERROR The function ptr is nullptr";
    }
    return unpackPrefabFun(engine, entityID);
}

bool EngineDLLInterface::savePrefab(EntityID entityID, const char* path) {
    if (savePrefabFun == nullptr) {
        std::cout << "EngineDLLInterface::savePrefab::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::savePrefab::ERROR The function ptr is nullptr";
    }
    return savePrefabFun(engine, entityID, path);
}

bool EngineDLLInterface::updatePrefabs(const char* path) {
    if (updatePrefabsFun == nullptr) {
        std::cout << "EngineDLLInterface::updatePrefabs::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::updatePrefabs::ERROR The function ptr is nullptr";
    }
    return updatePrefabsFun(engine, path);
}

bool EngineDLLInterface::updatePrefab(EntityID entityID) {
    if (updatePrefabFun == nullptr) {
        std::cout << "EngineDLLInterface::updatePrefab::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::updatePrefab::ERROR The function ptr is nullptr";
    }
    return updatePrefabFun(engine, entityID);
}

bool EngineDLLInterface::clearPropertyOverrides(EntityID entityID, TypeID typeID) {
    if (clearPropertyOverridesFun == nullptr) {
        std::cout << "EngineDLLInterface::clearPropertyOverrides::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::clearPropertyOverrides::ERROR The function ptr is nullptr";
    }
    return clearPropertyOverridesFun(engine, entityID, typeID);
}

bool EngineDLLInterface::createPrefabFromEntity(EntityID entityID, const char* path) {
    if (createPrefabFromEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::createPrefabFromEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createPrefabFromEntity::ERROR The function ptr is nullptr";
    }
    return createPrefabFromEntityFun(engine, entityID, path);
}

EntityID EngineDLLInterface::getNearestPrefabRootEntityID(EntityID entityID) {
    EntityID parentID = getEntityParent(entityID);
    if (parentID == 0) {
        return 0;
    }
    if (isEntityPrefabRoot(parentID)) {
        return parentID;
    }
    else {
        return getNearestPrefabRootEntityID(parentID);
    }
}

EntityID EngineDLLInterface::createPrefabEntity(const char* path, float x, float y) {
    if (createPrefabEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::createPrefabEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createPrefabEntity::ERROR The function ptr is nullptr";
    }
    return createPrefabEntityFun(engine, path, x, y);
}

EntityID EngineDLLInterface::createEntityFromPrefab(const char* path, float x, float y) {
    if (createEntityFromPrefabFun == nullptr) {
        std::cout << "EngineDLLInterface::createEntityFromPrefab::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createEntityFromPrefab::ERROR The function ptr is nullptr";
    }
    return createEntityFromPrefabFun(engine, path, x, y);
}

bool EngineDLLInterface::overrideProperty(EntityID entityID, std::size_t typeID, std::size_t propIndex) {
    if (overridePropertyFun == nullptr) {
        std::cout << "EngineDLLInterface::overrideProperty::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::overrideProperty::ERROR The function ptr is nullptr";
    }
    return overridePropertyFun(engine, entityID, typeID, propIndex);
}

bool EngineDLLInterface::removePropertyOverride(EntityID entityID, std::size_t typeID, std::size_t propIndex) {
    if (removePropertyOverrideFun == nullptr) {
        std::cout << "EngineDLLInterface::removePropertyOverride::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::removePropertyOverride::ERROR The function ptr is nullptr";
    }
    return removePropertyOverrideFun(engine, entityID, typeID, propIndex);
}

bool EngineDLLInterface::removeDestroyedEntityOverride(EntityID entityID, const char* guid) {
    if (removeDestroyedEntityOverrideFun == nullptr) {
        std::cout << "EngineDLLInterface::removeDestroyedEntityOverride::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::removeDestroyedEntityOverride::ERROR The function ptr is nullptr";
    }
    return removeDestroyedEntityOverrideFun(engine, entityID, guid);
}

bool EngineDLLInterface::isEntityPrefabRoot(EntityID entityID) {
    if (isEntityPrefabRootFun == nullptr) {
        std::cout << "EngineDLLInterface::isEntityPrefabRoot::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::isEntityPrefabRoot::ERROR The function ptr is nullptr";
    }
    return isEntityPrefabRootFun(engine, entityID);
}

bool EngineDLLInterface::isEntityAnOverride(EntityID entityID) {
    if (isEntityAnOverrideFun == nullptr) {
        std::cout << "EngineDLLInterface::isEntityAnOverride::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::isEntityAnOverride::ERROR The function ptr is nullptr";
    }
    return isEntityAnOverrideFun(engine, entityID);
}

bool EngineDLLInterface::isComponentOverriden(EntityID entityID, TypeID typeID) {
    auto componentOverrides = getComponentOverrides(entityID);
    return std::find_if(componentOverrides.begin(), componentOverrides.end(), [&typeID](const ComponentOverride& override) { return override.typeID == typeID; }) != componentOverrides.end();
}

bool EngineDLLInterface::isPropertyOverriden(EntityID entityID, TypeID typeID, std::string propertyName) {
    auto propertyOverrides = getPropertyOverrides(entityID, typeID);
    return std::find_if(propertyOverrides.begin(), propertyOverrides.end(), [&propertyName](const PropertyOverride& override) { return override.propertyName == propertyName; }) != propertyOverrides.end();
}

std::vector<PropertyOverride> EngineDLLInterface::getPropertyOverrides(EntityID entityID, TypeID typeID) {
    if (getPropertyOverridesFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyOverrides::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyOverrides::ERROR The function ptr is nullptr";
    }
    std::size_t overrideCount = getPropertyOverridesCount(entityID, typeID);
    std::vector<std::size_t> propertyIndices(overrideCount);
    getPropertyOverridesFun(engine, entityID, typeID, &propertyIndices[0], overrideCount);
    std::vector<PropertyOverride> overrides;
    overrides.resize(overrideCount);
    for (std::size_t i = 0; i < overrideCount; i++) {
        PropertyOverride & override = overrides[i];
        override.entityID = entityID;
        override.typeID = typeID;
        override.propertyName = getPropertyName(typeID, propertyIndices[i]);
    }
    return overrides;
}

std::vector<PropertyOverride> EngineDLLInterface::getPropertyOverridesAt(EntityID entityID) {
    if (getPropertyOverridesAtFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyOverridesAt::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyOverridesAt::ERROR The function ptr is nullptr";
    }
    std::size_t overrideCount = getPropertyOverridesAtCount(entityID);
    std::vector<EntityID> entityIDs(overrideCount);
    std::vector<TypeID> propertyTypeIDs(overrideCount);
    std::vector<std::size_t> propertyIndices(overrideCount);
    getPropertyOverridesAtFun(engine, entityID, &entityIDs[0], &propertyTypeIDs[0], &propertyIndices[0], overrideCount);
    std::vector<PropertyOverride> overrides;
    overrides.resize(overrideCount);
    for (std::size_t i = 0; i < overrideCount; i++) {
        auto& typeID = propertyTypeIDs[i];
        PropertyOverride& override = overrides[i];
        override.entityID = entityIDs[i];
        override.typeID = propertyTypeIDs[i];
        override.propertyName = getPropertyName(typeID, propertyIndices[i]);
    }
    return overrides;
}

std::vector<ComponentOverride> EngineDLLInterface::getComponentOverrides(EntityID entityID) {
    if (getComponentOverridesFun == nullptr) {
        std::cout << "EngineDLLInterface::getComponentOverrides::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getComponentOverrides::ERROR The function ptr is nullptr";
    }
    std::size_t overrideCount = getComponentOverridesCount(entityID);
    std::vector<std::size_t> typeIDs(overrideCount);
    getComponentOverridesFun(engine, entityID, &typeIDs[0], overrideCount);
    std::vector<ComponentOverride> overrides(overrideCount);
    for (std::size_t i = 0; i < overrideCount; i++) {
        ComponentOverride& override = overrides[i];
        override.entityID = entityID;
        override.typeID = typeIDs[i];
    }
    return overrides;
}

std::vector<std::string> EngineDLLInterface::getDestroyedEntityOverrides(EntityID entityID) {
    if (getDestroyedEntityOverridesFun == nullptr) {
        std::cout << "EngineDLLInterface::getDestroyedEntityOverrides::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getDestroyedEntityOverrides::ERROR The function ptr is nullptr";
    }
    std::size_t size = getDestroyedEntityOverridesFun(engine, entityID, readBuffer.data(), sizeof(readBuffer));
    std::vector<std::string> overrides;
    overrides.reserve(size);
    char* bufferPtr = readBuffer.data();
    for (std::size_t i = 0; i < size; i++) {
        std::string guid(bufferPtr);
        overrides.push_back(guid);
        bufferPtr += guid.size() + 1;
    }
    return overrides;
}

bool EngineDLLInterface::revertPrefab(EntityID entityID) {
    if (revertPrefabFun == nullptr) {
        std::cout << "EngineDLLInterface::revertPrefab::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::revertPrefab::ERROR The function ptr is nullptr";
    }
    return revertPrefabFun(engine, entityID);
}

std::string EngineDLLInterface::getPrefabFilePath(EntityID entityID) {
    if (getPrefabFilePathFun == nullptr) {
        std::cout << "EngineDLLInterface::getPrefabFilePath::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPrefabFilePath::ERROR The function ptr is nullptr";
    }
    getPrefabFilePathFun(engine, entityID, readBuffer.data(), sizeof(readBuffer));
    return std::string(readBuffer.data());
}

EntityID EngineDLLInterface::getPrefabOverrideReceiver(EntityID entityID) {
    if (getPrefabOverrideReceiverFun == nullptr) {
        std::cout << "EngineDLLInterface::getPrefabOverrideReceiver::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPrefabOverrideReceiver::ERROR The function ptr is nullptr";
    }
    return getPrefabOverrideReceiverFun(engine, entityID);
}

std::size_t EngineDLLInterface::getPropertyOverridesCount(EntityID entityID, TypeID typeID) {
    if (getPropertyOverridesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyOverridesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyOverridesCount::ERROR The function ptr is nullptr";
    }
    return getPropertyOverridesCountFun(engine, entityID, typeID);
}

std::size_t EngineDLLInterface::getPropertyOverridesAtCount(EntityID entityID) {
    if (getPropertyOverridesAtCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyOverridesAtCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyOverridesAtCount::ERROR The function ptr is nullptr";
    }
    return getPropertyOverridesAtCountFun(engine, entityID);
}

std::size_t EngineDLLInterface::getComponentOverridesCount(EntityID entityID) {
    if (getComponentOverridesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getComponentOverridesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getComponentOverridesCount::ERROR The function ptr is nullptr";
    }
    return getComponentOverridesCountFun(engine, entityID);
}

bool EngineDLLInterface::loadPropertyFromBuffer(EntityID entityID, TypeID typeID, const char* serializedData, std::size_t dataSize) {
    if (loadPropertyFromBufferFun == nullptr) {
        std::cout << "EngineDLLInterface::loadPropertyFromBuffer::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::loadPropertyFromBuffer::ERROR The function ptr is nullptr";
    }
    return loadPropertyFromBufferFun(engine, entityID, typeID, serializedData, dataSize);
}

bool EngineDLLInterface::loadComponentFromBuffer(EntityID entityID, const char* serializedData, std::size_t dataSize) {
    if (loadComponentFromBufferFun == nullptr) {
        std::cout << "EngineDLLInterface::loadComponentFromBuffer::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::loadComponentFromBuffer::ERROR The function ptr is nullptr";
    }
    return loadComponentFromBufferFun(engine, entityID, serializedData, dataSize);
}

bool EngineDLLInterface::loadEntityFromBuffer(const char* serializedData, std::size_t dataSize) {
    if (loadEntityFromBufferFun == nullptr) {
        std::cout << "EngineDLLInterface::loadEntityFromBuffer::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::loadEntityFromBuffer::ERROR The function ptr is nullptr";
    }
    return loadEntityFromBufferFun(engine, serializedData, dataSize);
}

std::string EngineDLLInterface::writePropertyToBuffer(EntityID entityID, TypeID typeID, std::size_t propIndex) {
    if (writePropertyToBufferFun == nullptr) {
        std::cout << "EngineDLLInterface::writePropertyToBuffer::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::writePropertyToBuffer::ERROR The function ptr is nullptr";
    }
    std::size_t size = writePropertyToBufferFun(engine, entityID, typeID, propIndex, readBuffer.data(), sizeof(readBuffer));
    return std::string(readBuffer.data(), size);
}

std::string EngineDLLInterface::writeComponentToBuffer(EntityID entityID, TypeID typeID) {
    if (writeComponentToBufferFun == nullptr) {
        std::cout << "EngineDLLInterface::writeComponentToBuffer::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::writeComponentToBuffer::ERROR The function ptr is nullptr";
    }
    std::size_t size = writeComponentToBufferFun(engine, entityID, typeID, readBuffer.data(), sizeof(readBuffer));
    return std::string(readBuffer.data(), size);
}

std::string EngineDLLInterface::writeEntityToBuffer(EntityID entityID) {
    if (writeEntityToBufferFun == nullptr) {
        std::cout << "EngineDLLInterface::writeEntityToBuffer::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::writeEntityToBuffer::ERROR The function ptr is nullptr";
    }
    std::size_t size = writeEntityToBufferFun(engine, entityID, readBuffer.data(), sizeof(readBuffer));
    return std::string(readBuffer.data(), size);
}

bool EngineDLLInterface::overrideParentComponent(EntityID entityID) {
    if (overrideParentComponentFun == nullptr) {
        std::cout << "EngineDLLInterface::overrideParentComponent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::overrideParentComponent::ERROR The function ptr is nullptr";
    }
    return overrideParentComponentFun(engine, entityID);
}

bool EngineDLLInterface::removeParentComponentOverride(EntityID entityID) {
    if (removeParentComponentOverrideFun == nullptr) {
        std::cout << "EngineDLLInterface::removeParentComponentOverride::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::removeParentComponentOverride::ERROR The function ptr is nullptr";
    }
    return removeParentComponentOverrideFun(engine, entityID);
}

bool EngineDLLInterface::isParentComponentOverriden(EntityID entityID) {
    if (isParentComponentOverridenFun == nullptr) {
        std::cout << "EngineDLLInterface::isParentComponentOverriden::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::isParentComponentOverriden::ERROR The function ptr is nullptr";
    }
    return isParentComponentOverridenFun(engine, entityID);
}

bool EngineDLLInterface::destroyEntity(EntityID entityID) {
    if (destroyEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::destroyEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::destroyEntity::ERROR The function ptr is nullptr";
    }
    return destroyEntityFun(engine, entityID);
}

std::vector<EntityID> EngineDLLInterface::getAllEntities() {
    if (getAllEntitiesFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllEntities::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllEntities::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getAllEntitiesCount();
    std::vector<EntityID> entities(outSize);
    if (getAllEntitiesFun(engine, &entities[0], outSize)) {
        return entities;
    }
    return std::vector<EntityID>();
}

std::size_t EngineDLLInterface::getAllEntitiesCount() {
    if (getAllEntitiesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllEntitiesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllEntitiesCount::ERROR The function ptr is nullptr";
    }
    return getAllEntitiesCountFun(engine);
}

void EngineDLLInterface::setEntitiesChangedCallback(EntitiesChangedCallbackFun fun) {
    if (setEntitiesChangedCallbackFun == nullptr) {
        std::cout << "EngineDLLInterface::setEntitiesChangedCallback::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setEntitiesChangedCallback::ERROR The function ptr is nullptr";
    }
    setEntitiesChangedCallbackFun(engine, fun);
}

void EngineDLLInterface::setCallbackPtr(void* ptr) {
    if (setCallbackPtrFun == nullptr) {
        std::cout << "EngineDLLInterface::setCallbackPtr::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setCallbackPtr::ERROR The function ptr is nullptr";
    }
    setCallbackPtrFun(engine, ptr);
}

bool EngineDLLInterface::addComponent(EntityID entityID, TypeID typeID) {
    if (addComponentFun == nullptr) {
        std::cout << "EngineDLLInterface::addComponent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::addComponent::ERROR The function ptr is nullptr";
    }
    return addComponentFun(engine, entityID, typeID);
}

bool EngineDLLInterface::removeComponent(EntityID entityID, TypeID typeID) {
    if (removeComponentFun == nullptr) {
        std::cout << "EngineDLLInterface::removeComponent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::removeComponent::ERROR The function ptr is nullptr";
    }
    return removeComponentFun(engine, entityID, typeID);
}

bool EngineDLLInterface::hasComponent(EntityID entityID, TypeID typeID) {
    if (hasComponentFun == nullptr) {
        std::cout << "EngineDLLInterface::hasComponent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::hasComponent::ERROR The function ptr is nullptr";
    }
    return hasComponentFun(engine, entityID, typeID);
}

bool EngineDLLInterface::setEntityName(EntityID entityID, const char* name) {
    if (setEntityNameFun == nullptr) {
        std::cout << "EngineDLLInterface::setEntityName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setEntityName::ERROR The function ptr is nullptr";
    }
    return setEntityNameFun(engine, entityID, name);
}

std::vector<ComponentData> EngineDLLInterface::getComponents(EntityID entityID) {
    if (getComponentsFun == nullptr) {
        std::cout << "EngineDLLInterface::getComponents::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getComponents::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getComponentsCount(entityID);
    std::vector<ComponentData> components(outSize);
    if (outSize > 0) {
        std::vector<void*> componentPtrs(outSize);
        std::vector<TypeID> componentTypeIDs(outSize);
        getComponentsFun(engine, entityID, &componentPtrs[0], &componentTypeIDs[0], outSize);
        for (std::size_t i = 0; i < outSize; i++) {
            components[i].instance = componentPtrs[i];
            components[i].typeID = componentTypeIDs[i];
        }
    }
    return components;
}

ComponentData EngineDLLInterface::getComponent(EntityID entityID, TypeID typeID) {
    std::vector<ComponentData> components = getComponents(entityID);
    for (const ComponentData& component : components) {
        if (component.typeID == typeID) {
            return component;
        }
    }
    std::cout << "EngineDLLInterface::getComponent::ERROR Found no matching component with typeID: " << typeID << std::endl;
    throw "EngineDLLInterface::getComponent::ERROR Found no matching component";
}

bool EngineDLLInterface::isEntityChild(EntityID entityID, EntityID parentID) {
    if (isEntityChildFun == nullptr) {
        std::cout << "EngineDLLInterface::isEntityChild::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::isEntityChild::ERROR The function ptr is nullptr";
    }
    return isEntityChildFun(engine, entityID, parentID);
}

bool EngineDLLInterface::setEntityParent(EntityID entityID, EntityID parentID) {
    if (setEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::setEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setEntityParent::ERROR The function ptr is nullptr";
    }
    return setEntityParentFun(engine, entityID, parentID);
}

bool EngineDLLInterface::hasEntityParent(EntityID entityID) {
    if (hasEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::hasEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::hasEntityParent::ERROR The function ptr is nullptr";
    }
    return hasEntityParentFun(engine, entityID);
}

bool EngineDLLInterface::detachEntityParent(EntityID entityID) {
    if (detachEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::detachEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::detachEntityParent::ERROR The function ptr is nullptr";
    }
    return detachEntityParentFun(engine, entityID);
}

std::string EngineDLLInterface::getEntityName(EntityID entityID) {
    if (getEntityNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityName::ERROR The function ptr is nullptr";
    }
    getEntityNameFun(engine, entityID, readBuffer.data(), sizeof(readBuffer));
    return std::string(readBuffer.data());
}

EntityID EngineDLLInterface::getEntityParent(EntityID entityID) {
    if (getEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityParent::ERROR The function ptr is nullptr";
    }
    return getEntityParentFun(engine, entityID);
}

EntityID EngineDLLInterface::getEntityChild(EntityID entityID, std::size_t index) {
    if (getEntityChildFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityChild::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityChild::ERROR The function ptr is nullptr";
    }
    return getEntityChildFun(engine, entityID, index);
}

EntityID EngineDLLInterface::getRootEntityID(EntityID entityID) {
    EntityID rootEntityID = getEntityParent(entityID);
    if (rootEntityID == 0) {
        return entityID;
    }
    return getRootEntityID(rootEntityID);
}

std::size_t EngineDLLInterface::getComponentsCount(EntityID entityID) {
    if (getComponentsCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getComponentsCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getComponentsCount::ERROR The function ptr is nullptr";
    }
    return getComponentsCountFun(engine, entityID);
}

std::size_t EngineDLLInterface::getEntityChildCount(EntityID entityID) {
    if (getEntityChildCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityChildCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityChildCount::ERROR The function ptr is nullptr";
    }
    return getEntityChildCountFun(engine, entityID);
}

std::size_t EngineDLLInterface::getEntityImmediateChildCount(EntityID entityID) {
    if (getEntityImmediateChildCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityImmediateChildCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityImmediateChildCount::ERROR The function ptr is nullptr";
    }
    return getEntityImmediateChildCountFun(engine, entityID);
}