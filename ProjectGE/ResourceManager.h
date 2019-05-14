#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Texture2D.h"
#include "Shader.h"
#include "FontManager.h"
#include "Font.h"
#include "Color.h"
#include <vector>

namespace Core {

	class ResourceManager
	{
	public:
		~ResourceManager();
		static ResourceManager& getInstance() {
			static ResourceManager instance;
			return instance;
		}
		Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile);
		Texture2D loadTexture(const GLchar* file, glm::ivec2 size = glm::ivec2(0,0), glm::ivec2 uvStartCoords = glm::ivec2(0,0));
		TextData2D createText(std::string text, Font font);
		glm::ivec2 getTextSize(const char* text, const Font& font);

		// Properly de-allocates all loaded resources
		void clear();
		// Updates Projection matrix of all shaders
		void updateShaders(glm::mat4& projection);
	private:
		ResourceManager();

		// Static  Loader functions
		static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile);
		static Texture2D loadTextureFromFile(const GLchar* file);

		// Private Loader functions
		FontManager& loadFontManager(const std::string file, GLushort textSize);

		//
		FT_Library ft;

		// Resource storage
		std::map<std::string, Shader> shaders;
		std::map<std::string, Texture2D> textures;
		std::map<std::string, FontManager*> fontManagers;
	};
}
#endif