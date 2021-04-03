#pragma once
#include <string>
#include <vector>
struct EnumInfo {
	std::string name;
	std::string sourceFile;
	std::vector<std::string> constants;
	std::vector<std::string> prettyConstants; // A list of the display names for the enum constants
};