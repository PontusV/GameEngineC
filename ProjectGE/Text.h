#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "Font.h"
#include "Component.h"
#include "glm/glm.hpp"

namespace GameEngine {
	class Texture;

	class Text : public Component {
	public:
		static constexpr ComponentTypeID TYPE_ID = 3;

		//static Text* getInstance(const std::string& text, std::string fontAddress, int fontSize, SDL_Color& color);
		//static Text* load(std::istream& is);

		Text(const std::string text, std::string fontAddress, int fontSize, glm::vec4 color);
		Text(std::istream& is);
		Text();

		~Text();
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		std::string& getText();
		Font& getFont();
		glm::vec4& getColor();

		//Save & Load operator
		void serialize(std::ostream &os) const;
		void deserialize(std::istream &is);
	private:
		std::string text;
		Font font; //Font address + size
		glm::vec4 color;
	};
}
#include "TypeList.h"
typedef typename GameEngine::TypeList<TYPE_LIST, GameEngine::Text>::type TypeListText;
#undef TYPE_LIST
#define TYPE_LIST TypeListText
#endif