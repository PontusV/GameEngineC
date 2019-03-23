#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace Core;


ResourceManager::ResourceManager()
{
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
}
ResourceManager::~ResourceManager()
{
	clear();
	FT_Done_FreeType(ft);
}

Shader ResourceManager::loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
	std::string name = std::string(vShaderFile) + "|" + fShaderFile;
	auto it = shaders.find(name);
	if (it != shaders.end())
		return it->second;
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return shaders[name];
}

Texture2D ResourceManager::loadTexture(const GLchar* file, glm::ivec2 size, glm::ivec2 uvStartPos)
{
	//Copy value from storage
	Texture2D texture;
	auto it = textures.find(file);
	if (it != textures.end()) {
		texture = it->second;
	} else {
		textures[file] = loadTextureFromFile(file);
		texture = textures[file];
	}

	//Modify uv positions of copy
	if (size.x == 0) size.x = texture.size.x;
	if (size.y == 0) size.y = texture.size.y;

	float uvWidth = (float)size.x / texture.size.x;
	float uvHeight = (float)size.y / texture.size.y;

	texture.uvPos[0] = uvStartPos;
	texture.uvPos[1] = glm::ivec2(uvStartPos.x, uvStartPos.y + uvHeight);
	texture.uvPos[2] = glm::ivec2(uvStartPos.x + uvWidth, uvStartPos.y + uvHeight);
	texture.uvPos[3] = glm::ivec2(uvStartPos.x + uvWidth, uvStartPos.y);

	return texture;
}

TextData2D ResourceManager::createText(std::string text, glm::vec4 color, Font font) {
	FontManager& manager = loadFontManager(font.getFileName());
	return manager.createText(text, color, font.getSize());
}

void ResourceManager::clear()
{
	// Delete all shaders	
	for (auto it : shaders)
		glDeleteProgram(it.second.ID);
	// Delete all textures
	for (auto it : textures)
		glDeleteTextures(1, &it.second.ID);
	// Delete all font managers
	for (auto it : fontManagers)
		delete it.second;
}

void ResourceManager::updateShaders(glm::mat4& projection) {
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		it->second.setMatrix4("projection", projection, true);
	}
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << vertexPath << "\n" << fragmentPath << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.compile(vShaderCode, fShaderCode);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* filePath)
{
	GLuint internalFormat = GL_RGB;
	GLuint imageFormat = GL_RGB;
	GLuint wrapS(GL_REPEAT);
	GLuint wrapT(GL_REPEAT);
	GLuint filterMin(GL_LINEAR);
	GLuint filterMax(GL_LINEAR);

	Texture2D texture;

	//Load image
	int width, height, nrChannels;
	glGenTextures(1, &texture.ID);

	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	texture.nrChannels = nrChannels;

	if (nrChannels == 1) {
		internalFormat = GL_RED;
		imageFormat = GL_RED;
	}
	else if (nrChannels == 3) {
		internalFormat = GL_RGB;
		imageFormat = GL_RGB;
	} 
	else if (nrChannels == 4) {
		internalFormat = GL_RGBA;
		imageFormat = GL_RGBA;
	}

	if (data)
	{
		texture.generate(width, height, internalFormat, imageFormat, wrapS, wrapT, filterMin, filterMax, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

FontManager& ResourceManager::loadFontManager(const std::string file) {
	auto it = fontManagers.find(file);
	if (it != fontManagers.end())
		return *it->second;

	static GLushort textSize = 48;

	fontManagers[file] = new FontManager(file.c_str(), textSize, ft);
	return *fontManagers[file];
}