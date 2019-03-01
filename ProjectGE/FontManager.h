#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>

#include "Texture2D.h"

namespace GameEngine {
	struct Character
	{
		glm::ivec2	size;		// Size of glyph
		glm::ivec2	bearing;	// Offset from baseline to left/top of glyph
		glm::vec2	uvPos;		// UV position in texture atlas
		GLuint		advance;	// Offset to advance to next glyph
	};

	//Texture2D with an offset position
	struct CharTexture2D {
		CharTexture2D() {}
		CharTexture2D(Texture2D texture, glm::vec2 offset) : texture(texture), offset(offset) {}
		Texture2D texture;
		glm::vec2 offset;
	};

	/* Manages a specified font. */
	class FontManager
	{
	public:
		FontManager() {}
		FontManager(const GLchar* fontFile, GLushort textSize, FT_Library ft);
		~FontManager();

		// Creates and returns a vector of textures for each character in the text parameter
		std::vector<CharTexture2D> createText(std::string text, glm::vec4 color, GLushort size);

	private:
		unsigned short textSize;
		//std::map<GLchar, Character>	characters;
		std::map<GLchar, CharTexture2D> textureMap;
		Character characters[128];
		CharTexture2D charTextures[128];
		Texture2D textureAtlas;
	};
}
#endif