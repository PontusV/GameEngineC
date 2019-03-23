#include "Image.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Chunk.h"
#include "Transform.h"

using namespace Core;
#include <iostream>

Image::Image(const char* fileName, unsigned short layerIndex, unsigned int width, unsigned int height) : GraphicComponent(layerIndex, width, height), fileName(fileName) {
} // Constructor
Image::Image(std::istream& is) {
	deserialize(is);
}
Image::Image() {
} 

Image::~Image() {
} // Destructor

void Image::reload() {
	if (texture.ID == 0) {
		texture = ResourceManager::getInstance().loadTexture(fileName.c_str());
		if (getSize().x != 0) {
			texture.size.x = getSize().x;
		}
		if (getSize().y != 0) {
			texture.size.y = getSize().y;
		}
	}
}

Texture2D& Image::getTexture() {
	return texture;
}

void Image::setTexture(const char* fileName, Texture2D& texture) {
	this->fileName = fileName;
	this->texture = texture;
}

const char* Image::getFileName() const {
	return fileName.c_str();
}

// Save & Load operator
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