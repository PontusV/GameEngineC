#pragma once
#include "ClassInfo.h"
#include <ReflectionParser/ReflectionTypes.h>
#include <vector>
#include <algorithm>

/* Returns the ClassInfo for all direct base classes of the given ClassInfo. */
std::vector<ClassInfo> getDirectBaseClasses(const ClassInfo& clazz, const std::vector<ClassInfo>& allClasses);
/* Returns the ClassInfo for all base classes of the given ClassInfo. */
std::vector<ClassInfo> getAllBaseClasses(const ClassInfo& clazz, const std::vector<ClassInfo>& allClasses);
/* Returns the ClassInfo for all derived classes of the given ClassInfo. */
std::vector<ClassInfo> getDerivedClasses(const ClassInfo& clazz, const std::vector<ClassInfo>& allClasses);
/* Returns a vector of all properties from the class, including all its inherited properties.
 * If any property has the same name as one that has already been added to the return value, it will be ignored.
 */
std::vector<Mirror::Property> getProperties(const ClassInfo& currentClass, const std::vector<ClassInfo>& baseClasses);
/* Returns a vector of all functions from the class, including all its inherited functions.
 * Excludes all overriden functions.
 */
std::vector<Mirror::Function> getFunctions(const ClassInfo& currentClass, const std::vector<ClassInfo>& baseClasses);
/* Returns true if the header extension is valid for a cpp header file. */
bool validHeaderExtension(std::string fileName);
/* Returns true if the header extension is valid for a cpp source file. */
bool validSourceExtension(std::string fileName);
/* Returns true if an equal value is contained by the vector. */
template<typename T>
bool contains(const std::vector<T>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}