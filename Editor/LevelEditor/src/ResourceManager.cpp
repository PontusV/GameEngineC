#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>

using namespace Editor;


ResourceManager::ResourceManager() {
}
ResourceManager::~ResourceManager() {
	clear();
}

Shader ResourceManager::loadShader(const std::string shaderFileName) {
	std::string vShaderFile = shaderFileName + ".vert";
	std::string fShaderFile = shaderFileName + ".frag";

	std::string name = vShaderFile + "|" + fShaderFile;
	auto it = shaders.find(name);
	if (it != shaders.end())
		return it->second;
	Shader shader = loadShaderFromFile(vShaderFile.c_str(), fShaderFile.c_str());
	updateShader(shader); // init projection matrix
	shaders[name] = shader;
	return shader;
}

void ResourceManager::clear()
{
	// Delete all shaders	
	for (auto it : shaders)
		glDeleteProgram(it.second.ID);
}

void ResourceManager::updateShader(Shader& shader) {
	updateShader(shader, projection);
}

void ResourceManager::updateShader(Shader& shader, const Matrix4& projection) {
	shader.setMatrix4("projection", projection, true);
}

void ResourceManager::updateShaders(const Matrix4& projection) {
	this->projection = projection;
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		updateShader(it->second, projection);
	}
}

void ResourceManager::updateShaderViewMatrix(const Matrix4& viewMatrix) {
	//this->projection = projection;
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		it->second.setMatrix4("model", viewMatrix, true);
	}
}

void ResourceManager::initShader(const Matrix4& projection) {
	updateShaders(projection);
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