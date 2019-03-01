#include "Image.h"
#include "Texture2D.h"
#include "ResourceManager.h"

using namespace GameEngine;

Image::Image(const std::string fileName, unsigned int width, unsigned int height) : fileName(fileName), width(width), height(height) {
} // Constructor
Image::Image(std::istream& is) {
	deserialize(is);
}
Image::Image() {
} // Destructor

Image::~Image() {}

void Image::reload() {
	if (texture.ID == 0) {
		texture = ResourceManager::getInstance().loadTexture(fileName.c_str());
		if (width != 0) texture.size.x = width;
		if (height != 0) texture.size.y = height;
	}
}
Texture2D& Image::getTexture() {
	return texture;
}

//Save & Load operator
void Image::serialize(std::ostream& os) const {
	//Save component ID; loaded in Graphics.cpp to identify which kind of Component to load
	os.write((char*)&TYPE_ID, sizeof(decltype(TYPE_ID)));
	//---------------------

	//Save address to image
	os.write(fileName.c_str(), fileName.size() + 1);
}
void Image::deserialize(std::istream& is) {
	//Load address to image
	std::getline(is, fileName, '\0');
}