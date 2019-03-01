#include "Text.h"
#include "Engine.h"
#include "Entity.h"
#include "Transform.h"
using namespace GameEngine;

Text::Text(const std::string text, std::string fontAddress, int fontSize, glm::vec4 color) : text(text), font(fontAddress, fontSize), color(color) {
} //Constructor

/* Used when loading this component from instream. Passes the instream to the Component part. */
Text::Text(std::istream& is) {
	deserialize(is);
}

/* Sets text to default text. */
//Text::Text() : Text("Default Text", "G:/ttf/cambriab.ttf", 30, SDL_Color{ 255,255,255 }) {
Text::Text() {
} //Constructor

Text::~Text() {
}

std::string& Text::getText() {
	return text;
}
Font& Text::getFont() {
	return font;
}
glm::vec4& Text::getColor() {
	return color;
}


/* Save */
void Text::serialize(std::ostream& os) const {
	//Save component ID; loaded in GameObject.cpp to identify which kind of Component to load
	os.write((char*)&TYPE_ID, sizeof(decltype(TYPE_ID)));
	//------------------------
	//Save text string
	os.write(text.c_str(), text.size() + 1);
	//Save font
	font.serialize(os);
	//Save color (red, green, blue, alpha)
	os.write((char*)&color.x, sizeof(float));
	os.write((char*)&color.y, sizeof(float));
	os.write((char*)&color.z, sizeof(float));
	os.write((char*)&color.w, sizeof(float));
}

/* Load */
void Text::deserialize(std::istream& is) {
	//Load text string
	std::getline(is, text, '\0'); //Read name
	//Load font
	font.deserialize(is);
	//Load color (red, blue, green, alpha)
	float r, b, g, a;
	is.read((char*)&r, sizeof(float));
	is.read((char*)&g, sizeof(float));
	is.read((char*)&b, sizeof(float));
	is.read((char*)&a, sizeof(float));
	color = glm::vec4(r,g,b,a);
}