#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>

#include "Texture2D.h"

namespace Core {
	struct Character
	{
		glm::ivec2		size;		// Size of glyph
		glm::ivec2		bearing;	// Offset from baseline to left/top of glyph
		glm::vec2		uvPos;		// UV position in texture atlas
		unsigned int	advance;	// Offset to advance to next glyph
	};

	struct Glyph {
		unsigned char advance;

		unsigned short width;
		unsigned short height;

		char bearingX;
		char bearingY;

		unsigned short textureX;
		unsigned short textureY;
	};

	//Texture2D with an offset position
	struct CharTexture2D {
		CharTexture2D() {}
		CharTexture2D(Texture2D texture, glm::vec2 offset) : texture(texture), offset(offset) {}
		Texture2D texture;
		glm::vec2 offset;
	};

	struct TextData2D {
		std::vector<CharTexture2D> textures;
		glm::ivec2 size;
	};

	/* Manages a specified font. */
	class FontManager
	{
	public:
		FontManager() {}
		FontManager(const char* fontFile, unsigned short textSize, FT_Library ft);
		~FontManager();

		// Creates and returns a vector of textures for each character in the text parameter
		TextData2D createText(std::string text, unsigned short size);

	private:
		Character characters[128];
		CharTexture2D charTextures[128];
		Texture2D textureAtlas;
		unsigned short textSize;
		unsigned short textHeight;
	};
}
#endif