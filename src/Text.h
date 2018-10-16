#ifndef TEXT_H
#define TEXT_H
#include <string>
#include "Font.h"
#include "ResourceManager.h"
#include "Component.h"
#include "HandleManager.h"

class Texture;

class Text : public Component {
public:
	static const int TYPE_ID = HandleManager::HANDLE_TYPE_COMPONENT_TEXT; //Variable used for Entity::GetComponent()

	static Text* getInstance(const std::string& text, std::string fontAddress, int fontSize, SDL_Color& color);
	static Text* load(std::istream& is);
	static Texture* loadTexture(const std::string& text, Font& font, SDL_Color color, SDL_Renderer* ren);

	~Text();
	void reload(SDL_Renderer* ren);
	TexturePtr getTexture();

	//Save & Load operator
	void serialize(std::ostream &os) const;
	void deserialize(std::istream &is);
private:
	Text(const std::string text, std::string fontAddress, int fontSize, SDL_Color color);
	Text();

	std::string text;
	Font font;
	SDL_Color color;
	TexturePtr texture;
};
#endif