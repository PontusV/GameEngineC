#pragma once
#include "ClassInfo.h"
#include "EnumInfo.h"
#include <ReflectionParser/ReflectionTypes.h>
#include <string>
#include <vector>
class Printer {
public:
	Printer(std::string target) : target(target) {}

	/* Prints new or modifies existing files for the given classes. Ends with printing a ReflectionPolymorph file for polymorphism support. */
	void print(const std::vector<ClassInfo>& classes);
	void printEnums(const std::vector<EnumInfo>& enumerations);

private:
	void printHeaderFile(std::string fileName, std::ostringstream& osstream);
	void printSourceFile(std::string fileName, std::ostringstream& osstream);
	/* Creates a new file or modifies an existing file. */
	void printFile(std::string fileName, std::ostringstream& osstream);
	/* Creates a new file or modifies an existing file with updated reflection data for the given class. */
	void printClass(const ClassInfo& currentClass, const std::vector<ClassInfo>& baseClasses, const std::vector<ClassInfo>& directBaseClasses);
	/* Creates a new file or modifies an existing file with updated functionality for all reflected classes. */
	void printReflectionPolymorph(const std::vector<ClassInfo>& classes);

	void printPropertyDetails(const Mirror::Property& prop, std::string variableName, std::ostream& os);
	void printProperty(const Mirror::Property& currentProperty, std::ostream& os, bool& newProperty);
	void printPropertyType(const Mirror::VariableType& type, std::ostream& os, std::string varName);
	void printFunction(const Mirror::Function& currentFunction, std::ostream& os, bool& newFunction, bool& newParameter);
	void printGetValue(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os);
	void printSetValue(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os);
	void printOnUpdate(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os);
	void printInvoke(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os);
	void printGetPointer(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os);
	void printGetArraySize(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os);

	/* Utility used for increased readability of generated files */
	std::string printTabs(std::size_t amount) const;

private:
	std::string target;
};