#include "FontManager.h"
#include "ResourceManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

using namespace Core;


FontManager::FontManager(const char* fontFile, unsigned short textSize, FT_Library ft) : textSize(textSize)
{
	FT_Face face;
	if (FT_New_Face(ft, fontFile, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}

	FT_Set_Char_Size(face, 0, textSize*64, 1960, 1080);
	FT_Set_Pixel_Sizes(face, 0, textSize);

	//textHeight = face->size->metrics.height >> 6;
	textHeight = (int)(textSize * 0.6875f);


	//------
	FT_GlyphSlot g = face->glyph;
	unsigned int atlas_width = 0;
	unsigned int atlas_height = 0;

	// Calculate atlas size
	for (std::size_t i = 0; i < FONT_MANAGER_CHARACTER_COUNT; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		atlas_width += g->bitmap.width;
		atlas_height = std::max(atlas_height, g->bitmap.rows);
	}
	textureAtlas.size = Vector2(atlas_width, atlas_height);


	// Prepare new atlas texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureAtlas.ID);
	glBindTexture(GL_TEXTURE_2D, textureAtlas.ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Makes it blurry
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Makes it blurry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Makes the text look weird (Texture cut off)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Makes the text look weird (Texture cut off)

	float x = 0;
	float y = 0;

	for (std::size_t i = 0; i < FONT_MANAGER_CHARACTER_COUNT; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Creating texture for character %c failed!\n", i);
			continue;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLuint)x, (GLuint)y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		characters[i].size = Vector2(g->bitmap.width, g->bitmap.rows);
		characters[i].bearing = Vector2(g->bitmap_left, g->bitmap_top);
		characters[i].uvPos = Vector2((x/*+0.5f*/)/atlas_width, 0.0f);
		characters[i].advance = g->advance.x >> 6;

		x += g->bitmap.width;
		//y = 0;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
}


FontManager::~FontManager()
{
	glDeleteTextures(1, &textureAtlas.ID);
}


TextData2D FontManager::createText(std::wstring text, unsigned short size) {
	//float scale = ((float)size)/textSize;
	//float scale = 1.0f;

	//Offsets start at 0
	GLfloat x = 0;
	GLfloat y = 0;
	
	std::vector<CharTexture2D> textures;
	textures.reserve(text.size());

	float width = 0.0f;

	// Iterate through all characters
	for (std::size_t character : text) {
		Character& ch = characters[character];
		if (charTextures[character].texture.ID == 0) { // ID is zero if uninitialized

			Texture2D tex = textureAtlas;
			Vector2 offset(x + ch.bearing.x, y - ch.bearing.y);

			tex.size = ch.size;

			float uvWidth = (float)ch.size.x / textureAtlas.size.x;
			float uvHeight = (float)ch.size.y / textureAtlas.size.y;

			tex.uvCoords[0] = Vector2(ch.uvPos.x, ch.uvPos.y);
			tex.uvCoords[1] = Vector2(ch.uvPos.x, ch.uvPos.y + uvHeight);
			tex.uvCoords[2] = Vector2(ch.uvPos.x + uvWidth, ch.uvPos.y + uvHeight);
			tex.uvCoords[3] = Vector2(ch.uvPos.x + uvWidth, ch.uvPos.y);

			charTextures[character] = CharTexture2D(tex, offset);
		}

		//Copy texture from storage and modify offset and size to scale
		CharTexture2D texture = charTextures[character];
		texture.offset.x = x + (float)ch.bearing.x /* scale*/;
		texture.offset.y = y - (float)ch.bearing.y /* scale*/;
		texture.texture.size.x = (int)(texture.texture.size.x /* scale*/);
		texture.texture.size.y = (int)(texture.texture.size.y /* scale*/);
		textures.push_back(texture);

		x += (float)ch.advance /* scale*/;
		width = x;
	}

	//int height = (int)(textHeight * scale);
	int height = textHeight;

	return { textures, Vector2(width,height) };
}