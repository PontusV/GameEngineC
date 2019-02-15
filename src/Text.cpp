#include "Text.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Engine.h"
#include "Entity.h"
#include "Transform.h"
using namespace GameEngine;


Text::Text(const std::string text, std::string fontAddress, int fontSize, SDL_Color color) : text(text), font(fontAddress, fontSize), color(color) {

} //Constructor

Text::Text(std::istream& is) {

}

/* Used when loading this component from instream. Passes the instream to the Component part. */
Text::Text() {
} //Constructor

Text::~Text() {
}

/* Loads texture if TexturePtr is null. Draws the text on the given renderer */
void Text::reload(SDL_Renderer* ren) {
	//Check if text is loaded
	if (texture == nullptr)
		texture = ResourceManager::getInstance().loadText(text, font, color, ren); //Attempt to load image
}

TexturePtr Text::getTexture() {
	return texture;
}


/* Save */
void Text::serialize(std::ostream& os) const {
	//Save component ID; loaded in GameObject.cpp to identify which kind of Component to load
	os << TYPE_ID;
	//------------------------
	//Save text string
	os.write(text.c_str(), text.size() + 1);
	//Save font
	os << font;
	//Save color (red, blue, green, alpha)
	os << color.r;
	os << color.b;
	os << color.g;
	os << color.a;
}

void Text::deserialize(std::istream& is) {
	//Load text string
	std::getline(is, text, '\0'); //Read name
	//Load font
	is >> font;
	//Load color (red, blue, green, alpha)
	Uint8 r, b, g, a;
	is >> r;
	is >> b;
	is >> g;
	is >> a;
	SDL_Color color = { r,b,g,a };
}

/* Load class from input stream (ex: hard drive). */
std::istream& operator>>(std::istream& is, Text& object) {
	object.deserialize(is);
	return is;
}

/* Save class to output stream (ex: hard drive). */
std::ostream& operator<<(std::ostream& os, const Text& object) {
	object.serialize(os);
	return os;
}





/*Text* Text::load(std::istream& is) {
	Text* text = new Text();
	text->deserialize(is);
	return text;
}

Text* Text::getInstance(const std::string& text, std::string fontAddress, int fontSize, SDL_Color& color) {
	return new Text(text, fontAddress, fontSize, color);
}*/