#ifndef TEXT_H
#define TEXT_H


#include <string>
#include "Font.h"
#include "Sprite.h"
#include "Color.h"
#include <glm/glm.hpp>

namespace Core {
	class Text : public Sprite {
	public:

		Text(std::string text, const char* fontAddress, int fontSize, Color color, unsigned char layerIndex = 0);
		Text();
		~Text();

		const char* getText() const;
		const Font& getFont() const;

		void setText(std::string text);

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	private:
		std::string		text;	// Replace with vector of Images?
		Font			font;	//Font address + size
	};
}
#endif