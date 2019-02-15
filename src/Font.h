#ifndef FONT_H
#define FONT_H
#include <string>
#include "SDL_ttf.h"
#include "Serializable.h"

namespace GameEngine {
	/* Stores a TTF_Font and the variables used to create it. Used to save and load font from file */
	class Font : public Serializable {
	public:
		Font(std::string fileName, int size);
		Font();
		~Font();
		TTF_Font* getFont();
		const std::string& getFileName() const;
		const int& getSize() const;
		//Save & Load operator
		void serialize(std::ostream &os) const;
		void deserialize(std::istream &is);
	private:
		std::string fileName;
		int size;
		TTF_Font* font;
	};
	std::istream& operator>>(std::istream& is, Font& object);
	std::ostream& operator<<(std::ostream& os, const Font& object);
}
#endif