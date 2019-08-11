#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Maths/Vector2.h"
#include <vector>
#include <map>
#include <string>

#include "Texture2D.h"

#define FONT_MANAGER_CHARACTER_COUNT 255

namespace Core {
	struct Character
	{
		Vector2 size;		// Size of glyph
		Vector2 bearing;	// Offset from baseline to left/top of glyph
		Vector2 uvPos;		// UV position in texture atlas
		unsigned int advance;	// Offset to advance to next glyph
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
		CharTexture2D(Texture2D texture, Vector2 offset) : texture(texture), offset(offset) {}
		Texture2D texture;
		Vector2 offset;
	};

	struct TextData2D {
		std::vector<CharTexture2D> textures;
		Vector2 size;
	};

	/* Manages a specified font. */
	class FontManager
	{
	public:
		FontManager() {}
		FontManager(const char* fontFile, unsigned short textSize, FT_Library ft);
		~FontManager();

		// Creates and returns a vector of textures for each character in the text parameter
		TextData2D createText(std::wstring text, unsigned short size);

	private:
		Character characters[FONT_MANAGER_CHARACTER_COUNT];
		CharTexture2D charTextures[FONT_MANAGER_CHARACTER_COUNT];
		Texture2D textureAtlas;
		unsigned short textSize;
		unsigned short textHeight;
	};
}
#endif