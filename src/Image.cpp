#include "Image.h"
#include "Texture.h"



Image::Image(const std::string fileName) : fileName(fileName) {
} //Constructor
Image::Image(std::istream& is) { //Load image from instream(file)
	deserialize(is);
} //Constructor
/* Used when loading this component from instream. Passes the instream to the Component part. */
Image::Image() {
} //Constructor

Image::~Image() { //Destructor
}

/* Loads texture if TexturePtr is null. Draws the image on the given renderer. */
void Image::reload(SDL_Renderer* ren) {
	//Check if image is loaded
	if (texture == nullptr)
		texture = ResourceManager::getInstance().loadTexture(fileName, ren); //Attempt to load image
}

TexturePtr Image::getTexture() {
	return texture;
}

/* Save */
void Image::serialize(std::ostream& os) const {
	//Save component ID; loaded in Graphics.cpp to identify which kind of Component to load
	os << Component::componentTypes::Image;
	//---------------------

	//Save address to image
	os.write(fileName.c_str(), fileName.size() + 1);
}

void Image::deserialize(std::istream& is) {
	//Load address to image
	std::getline(is, fileName, '\0');
}

/* Load class from input stream (ex: hard drive). */
std::istream& operator>>(std::istream& is, Image& object) { //Does not do anything
	object.deserialize(is);
	return is;
}

/* Save class to output stream (ex: hard drive). */
std::ostream& operator<<(std::ostream& os, const Image& object) {
	object.serialize(os);
	return os;
}