#include "Font.h"
#include <stdexcept>
using namespace Core;


const char* Font::defaultAddress	= "G:/ttf/cambriab.ttf";
int Font::defaultSize				= 30;

Font::Font(const char* fileName, int size) : fileName(fileName), size(size) {
}

Font::~Font() {
}

Font::Font() {
	//Set a default font?
}

const char* Font::getFileName() const {
	return fileName.c_str();
}

const int& Font::getSize() const {
	return size;
}


/* Save */
void Font::serialize(std::ostream& os) const {
	os.write(fileName.c_str(), fileName.size() + 1);	//Write name
	os.write((char*)&size, sizeof(decltype(size)));		//Write size
}

/* Load */
void Font::deserialize(std::istream& is) {
	std::getline(is, fileName, '\0');					//Read name
	is.read((char*)&size, sizeof(decltype(size)));		//Read size
}