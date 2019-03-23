#ifndef TEXT_H
#define TEXT_H


#include <string>
#include "Font.h"
#include "GraphicComponent.h"
#include "glm/glm.hpp"

namespace Core {
	class Text : public GraphicComponent {
	public:
		static constexpr ComponentTypeID TYPE_ID = 3;
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		Text(const char* text, const char* fontAddress, int fontSize, glm::vec4 color, unsigned short layerIndex = 0);
		Text(std::istream& is);
		Text();
		~Text();

		const char* getText() const;
		const Font& getFont() const;
		const glm::vec4& getColor() const;

		void setText(std::string text);

		//Save & Load operator
		void serialize(std::ostream &os) const;
		void deserialize(std::istream &is);
	private:
		std::string		text;
		Font			font;	//Font address + size
		glm::vec4		color;
	};
}
#endif