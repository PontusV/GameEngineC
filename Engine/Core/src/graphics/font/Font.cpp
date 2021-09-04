#include "Font.h"
#include <stdexcept>
using namespace Core;


const char* Font::defaultAddress	= "G:/ttf/cambriab.ttf";
int Font::defaultSize				= 30;

Font::Font(const char* fileName, int size) : fileName(fileName), size(size) {
}

Font::~Font() {
}

Font::Font() : fileName(""), size(defaultSize) {
	//Set a default font?
}

const char* Font::getFileName() const {
	return fileName.c_str();
}

const int& Font::getSize() const {
	return size;
}