#include "FontManager.h"
#include "ResourceManager.h"
#include <algorithm>

using namespace Core;


FontManager::FontManager(const GLchar* fontFile, GLushort textSize, FT_Library ft) : textSize(textSize)
{
	FT_Face face;
	if (FT_New_Face(ft, fontFile, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, textSize);

	//textHeight = face->size->metrics.height >> 6;
	textHeight = (int)(textSize * 0.6875f);


	//------
	FT_GlyphSlot g = face->glyph;
	unsigned int atlas_width = 0;
	unsigned int atlas_height = 0;

	// Calculate atlas size
	for (std::size_t i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		atlas_width += g->bitmap.width + 1; // +1 to compensate for UV positions rounding down
		atlas_height = std::max(atlas_height, g->bitmap.rows);
	}
	textureAtlas.size = glm::ivec2(atlas_width, atlas_height);


	// Prepare new atlas texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureAtlas.ID);
	glBindTexture(GL_TEXTURE_2D, textureAtlas.ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int x = 0;
	int y = 0;

	for (std::size_t i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Creating texture for character %c failed!\n", i);
			continue;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		characters[i].size = glm::ivec2(g->bitmap.width, g->bitmap.rows);
		characters[i].bearing = glm::ivec2(g->bitmap_left, g->bitmap_top);
		characters[i].uvPos = glm::vec2((float)x/atlas_width, (float)y/atlas_height);
		characters[i].advance = g->advance.x >> 6;

		x += g->bitmap.width + 1; // +1 to compensate for UV positions rounding down
		//y = 0;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
}


FontManager::~FontManager()
{
	glDeleteTextures(1, &textureAtlas.ID);
}


TextData2D FontManager::createText(std::string text, glm::vec4 color, GLushort size) {
	float scale = ((float)size)/textSize;

	//Offsets start at 0
	GLfloat x = 0;
	GLfloat y = 0;
	
	std::vector<CharTexture2D> textures;
	textures.reserve(text.size());

	float width;

	// Iterate through all characters
	for (std::size_t character : text) {
		Character& ch = characters[character];
		if (charTextures[character].texture.ID == 0) { // ID is zero if uninitialized

			Texture2D tex = textureAtlas;
			glm::vec2 offset(x + ch.bearing.x, y - ch.bearing.y);

			tex.size = ch.size;

			float uvWidth = (float)ch.size.x / textureAtlas.size.x;
			float uvHeight = (float)ch.size.y / textureAtlas.size.y;

			tex.uvPos[0] = glm::vec2(ch.uvPos.x, ch.uvPos.y);
			tex.uvPos[1] = glm::vec2(ch.uvPos.x, ch.uvPos.y + uvHeight);
			tex.uvPos[2] = glm::vec2(ch.uvPos.x + uvWidth, ch.uvPos.y + uvHeight);
			tex.uvPos[3] = glm::vec2(ch.uvPos.x + uvWidth, ch.uvPos.y);

			charTextures[character] = CharTexture2D(tex, offset);
		}

		//Copy texture from storage and modify offset and size to scale
		CharTexture2D texture = charTextures[character];
		texture.offset.x = x + ch.bearing.x * scale;
		texture.offset.y = y - ch.bearing.y * scale;
		texture.texture.size.x = (int)(texture.texture.size.x * scale);
		texture.texture.size.y = (int)(texture.texture.size.y * scale);
		textures.push_back(texture);

		width = x;
		x += ch.advance * scale;
	}

	int height = (int)(textHeight * scale);

	return { textures, glm::ivec2(width,height) };
}