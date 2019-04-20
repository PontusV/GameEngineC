#ifndef TEXT_H
#define TEXT_H


#include <string>
#include "Font.h"
#include "GraphicComponent.h"
#include "glm/glm.hpp"

namespace Core {
	class Text : public GraphicComponent {
	public:

		Text(const char* text, const char* fontAddress, int fontSize, glm::vec4 color, unsigned char layerIndex = 0);
		Text();
		~Text();

		const char* getText() const;
		const Font& getFont() const;
		const glm::vec4& getColor() const;

		void setText(std::string text);

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	private:
		std::string		text;
		Font			font;	//Font address + size
		glm::vec4		color;
	};
}
#endif