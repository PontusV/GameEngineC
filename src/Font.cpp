#include "Font.h"
using namespace GameEngine;


Font::Font(std::string fileName, int size) : fileName(fileName), size(size) {
	font = TTF_OpenFont(fileName.c_str(), size);
	std::cout << "\nOpened Font: " << font;
}

Font::~Font() {
	std::cout << "\nClosing Font: " << font;
	if (font != nullptr)
		TTF_CloseFont(font);
}

Font::Font() {
}

TTF_Font* Font::getFont() {
	return font;
}

const std::string& Font::getFileName() const {
	return fileName;
}

const int& Font::getSize() const {
	return size;
}


/* Save */
void Font::serialize(std::ostream& os) const {
	os.write(fileName.c_str(), fileName.size() + 1); //Write name
	os << size;
}

/* Load */
void Font::deserialize(std::istream& is) {
	std::getline(is, fileName, '\0'); //Read name
	is >> size;
	font = TTF_OpenFont(fileName.c_str(), size);
	std::cout << "\n(L)Opened Font: " << font;
}

/* Load class from input stream (ex: hard drive). */
std::istream& GameEngine::operator>>(std::istream& is, Font& font) {
	font.deserialize(is);
	return is;
}

/* Save class to output stream (ex: hard drive). */
std::ostream& GameEngine::operator<<(std::ostream& os, const Font& font) {
	font.serialize(os);
	return os;
}