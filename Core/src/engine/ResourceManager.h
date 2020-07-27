#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "maths/Vector2.h"
#include "maths/Matrix4.h"
#include "graphics/data/Texture2D.h"
#include "graphics/data/Shader.h"
#include "graphics/font/FontManager.h"
#include "graphics/font/Font.h"
#include "graphics/data/Color.h"
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
		/* @param shaderFileName the name of the shader file without any file extension. */
		Shader loadShader(const std::string shaderFileName);
		Texture2D loadTexture(const GLchar* file, Vector2 size = Vector2(0,0), Vector2 uvStartCoords = Vector2(0,0));
		TextData2D getTextData(std::wstring text, Font font);
		Vector2 getTextSize(std::wstring text, const Font& font);

		// De-allocates all loaded resources
		void clear();
		void updateShader(Shader& shader);
		void updateShader(Shader& shader, const Matrix4& projection);
		// Updates Projection matrix of all shaders
		void updateShaders(const Matrix4& projection);
		// Updates View matrix of all loaded shaders
		void updateShaderViewMatrix(const Matrix4& viewMatrix);
		// Initializes all shaders. Prepares them for use in rendering.
		void initShader(const Matrix4& projection);
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
		Matrix4 projection;
	};
}
#endif