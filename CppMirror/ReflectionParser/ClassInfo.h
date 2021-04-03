#pragma once
#include <ReflectionParser/ReflectionTypes.h>
#include <string>
#include <cstddef>
struct ClassInfo : public Mirror::Class {
	ClassInfo(std::size_t typeID) : Mirror::Class(typeID) {}
	std::string sourceFile;
	std::string fullName;
};