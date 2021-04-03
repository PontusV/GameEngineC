#include "Parser.h"
#include "Printer.h"
#include "Util.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <string>
#include <fstream>
#include <stdexcept>

bool isIgnored(const std::vector<std::string>& ignoreList, const std::string& value) {
	for (const std::string& ignorePath : ignoreList) {
		if (value.find(ignorePath) == 0)
			return true;
	}
	return false;
}

int main(int argc, char** argv) {
	std::cout << "Parser initializing!\n";
	std::vector<const char*> cmdArguments;
	std::vector<std::string> parseTargets;
	std::string printTarget = "";
	for (int i = 0; i < argc; i++) {
		std::string current = argv[i];
		if (current.rfind("-T", 0) == 0) { // If target
			current.erase(0, 2);
			std::replace(current.begin(), current.end(), '\\', '/');
			std::cout << "Parse target: " << current << "\n";
			parseTargets.push_back(current);
		}
		else if (current.rfind("-P", 0) == 0) { // If print path
			current.erase(0, 2);
			std::replace(current.begin(), current.end(), '\\', '/');
			std::cout << "Print path: " << current << "\n";
			printTarget = current;
		}
		else {
			cmdArguments.push_back(argv[i]);
		}
	}

	if (printTarget.empty()) {
		std::cout << "No print path was specified!" << std::endl;
		exit(-1);
	}

	if (parseTargets.size() == 0) {
		std::cout << "No parse target was specified!" << std::endl;
		exit(-1);
	}

	cmdArguments.push_back("-DREFLECTION_PARSER");
	cmdArguments.push_back("-x");
	cmdArguments.push_back("c++");
	cmdArguments.push_back("-std=c++17");

	std::size_t typeIDCounter = 1;
	Parser parser;
	Printer printer(printTarget);
	std::cout << "Parse starting!\n";
	for (std::string& target : parseTargets) {
		if (validHeaderExtension(target)) {
			// Parse header
			try {
				parser.parseTranslationUnit(target.c_str(), cmdArguments, typeIDCounter);
			}
			catch (std::exception& e) {
				std::cout << "Parser Error: " << e.what() << std::endl;
				std::cout << "Parser stopping..." << std::endl;
			}
		}
		else if (std::filesystem::is_directory(target)) {
			// ---- Parse directory ----
			std::string str;
			// Read ignore (Mirror.ignore)
			std::ifstream ignoreFile(std::string(target) + "Mirror.ignore", std::ifstream::in);
			std::vector<std::string> ignoredFiles;
			while (std::getline(ignoreFile, str)) {
				std::replace(str.begin(), str.end(), '\\', '/');
				if (str.back() == '*') str = str.substr(0, str.size() - 2);
				ignoredFiles.push_back(target + str);
			}
			ignoredFiles.push_back(target + "ReflectionTemp/ReflectionTargets.generated.h");
			// Read include (Mirror.include)
			std::ifstream includeFile(std::string(target) + "Mirror.include", std::ifstream::in);
			std::vector<std::string> includedFiles;
			while (std::getline(includeFile, str)) {
				if (str.back() == '*') str = str.substr(0, str.size() - 1);
				std::replace(str.begin(), str.end(), '\\', '/');
				if (std::filesystem::is_directory(str)) {
					// Iterate through directory
					for (const auto& entry : std::filesystem::recursive_directory_iterator(str)) {
						std::string pathName = entry.path().string();
						if (validHeaderExtension(pathName)) {
							std::replace(pathName.begin(), pathName.end(), '\\', '/');
							includedFiles.push_back(target + pathName);
						}
					}
				}
				else if (validHeaderExtension(str)) {
					includedFiles.push_back(target + str);
				}
			}

			// Iterate through includedFiles
			std::string reflectionTempDir = target + "/ReflectionTemp";
			std::string reflectionTempFileName = reflectionTempDir + "/ReflectionTargets.generated.h";
			std::filesystem::create_directory(reflectionTempDir);
			std::ofstream outfile(reflectionTempFileName, std::ofstream::out | std::ofstream::trunc);
			for (const std::string& includePath : includedFiles) {
				// Include files bypass ignore check
				outfile << "#include \"" << includePath << "\"" << std::endl;
			}
			// Iterate through entries in target folder
			for (const auto& entry : std::filesystem::recursive_directory_iterator(target)) {
				std::string pathName = entry.path().string();
				if (validHeaderExtension(pathName)) {
					std::replace(pathName.begin(), pathName.end(), '\\', '/');
					// Check if file should be ignored
					if (!isIgnored(ignoredFiles, pathName)) {
						outfile << "#include \"" << pathName << "\"" << std::endl;
					}
				}
			}
			outfile.close();

			try {
				parser.parseTranslationUnit(reflectionTempFileName.c_str(), cmdArguments, typeIDCounter);
			}
			catch (std::exception& e) {
				std::cout << "Parser Error: " << e.what() << std::endl;
				std::cout << "Parser stopping..." << std::endl;
				remove(reflectionTempFileName.c_str());
				std::filesystem::remove(reflectionTempDir);
				return 0;
			}
			remove(reflectionTempFileName.c_str());
			std::filesystem::remove(reflectionTempDir);
		}
		else {
			std::cout << "Invalid parse target: " << target << std::endl;
			continue;
		}
	}
	// Print all classes
	printer.print(parser.getClasses());
	printer.printEnums(parser.getEnumerations());

	std::cout << "Parse successful!\n";
	return 0;
}