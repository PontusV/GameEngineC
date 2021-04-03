#include "Util.h"

using namespace Mirror;


std::vector<ClassInfo> getDirectBaseClasses(const ClassInfo& clazz, const std::vector<ClassInfo>& allClasses) {
	std::vector<ClassInfo> baseClasses;
	for (const ClassInfo& base : allClasses) {
		for (const Mirror::Type& type : clazz.baseClasses) {
			if (type.name == base.fullName) {
				baseClasses.push_back(base);
				continue;
			}
		}
	}
	return baseClasses;
}

std::vector<ClassInfo> getAllBaseClasses(const ClassInfo& clazz, const std::vector<ClassInfo>& allClasses) {
	std::vector<ClassInfo> baseClasses;
	for (const ClassInfo& base : allClasses) {
		for (const Mirror::Type& type : clazz.baseClasses) {
			if (type.name == base.fullName) {
				baseClasses.push_back(base);
				continue;
			}
		}
	}
	// Get base classes of base classes
	std::vector<ClassInfo> additional;
	for (const ClassInfo& base : baseClasses) {
		std::vector<ClassInfo> bases = getAllBaseClasses(base, allClasses); // May cause infinite loop
		for (const ClassInfo& add : bases) {
			additional.push_back(add);
		}
	}
	for (const ClassInfo& add : additional) {
		baseClasses.push_back(add);
	}
	return baseClasses;
}

std::vector<ClassInfo> getDerivedClasses(const ClassInfo& clazz, const std::vector<ClassInfo>& allClasses) {
	std::vector<ClassInfo> derivedClasses;
	for (const ClassInfo& derived : allClasses) {
		for (const Mirror::Type& type : derived.baseClasses) {
			if (type.name == clazz.fullName) {
				derivedClasses.push_back(derived);
				continue;
			}
		}
	}
	// Get derived classes of derived classes
	std::vector<ClassInfo> additional;
	for (const ClassInfo& derived : derivedClasses) {
		std::vector<ClassInfo> derivedVec = getDerivedClasses(derived, allClasses); // Make cause infinite loop
		for (const ClassInfo& add : derivedVec) {
			additional.push_back(add);
		}
	}
	for (const ClassInfo& add : additional) {
		derivedClasses.push_back(add);
	}
	return derivedClasses;
}

std::vector<Mirror::Property> getProperties(const ClassInfo& currentClass, const std::vector<ClassInfo>& baseClasses) {
	std::vector<Mirror::Property> properties;
	for (const Mirror::Property& currentProperty : currentClass.properties) {
		properties.push_back(currentProperty);
	}
	for (const ClassInfo& baseClass : baseClasses) {
		for (const Mirror::Property& currentProperty : baseClass.properties) {
			bool add = true;
			for (const Mirror::Property& prop : properties) {
				if (currentProperty.name == prop.name) {
					add = false;
					break;
				}
			}
			if (add)
				properties.push_back(currentProperty);
		}
	}
	return properties;
}

std::vector<Mirror::Function> getFunctions(const ClassInfo& currentClass, const std::vector<ClassInfo>& baseClasses) {
	std::vector<Mirror::Function> functions;
	for (const Mirror::Function& currentFunction : currentClass.functions) {
		functions.push_back(currentFunction);
	}
	for (const ClassInfo& baseClass : baseClasses) {
		for (const Mirror::Function& inherited : baseClass.functions) {
			bool add = true;
			for (const Mirror::Function& fun : functions) {
				if (fun == inherited) { // Checks if function is overriden in current class
					add = false;
					break;
				}
			}
			if (add)
				functions.push_back(inherited);
		}
	}
	return functions;
}

bool validHeaderExtension(std::string fileName) {
	std::size_t extensionIndex = fileName.find_last_of(".");
	if (extensionIndex != std::string::npos) {
		std::string fileExtension = fileName.substr(extensionIndex + 1);
		return fileExtension == "h" || fileExtension == "hpp";
	}
	return false;
}

bool validSourceExtension(std::string fileName) {
	std::size_t extensionIndex = fileName.find_last_of(".");
	if (extensionIndex != std::string::npos) {
		std::string fileExtension = fileName.substr(extensionIndex + 1);
		return fileExtension == "cpp";
	}
	return false;
}