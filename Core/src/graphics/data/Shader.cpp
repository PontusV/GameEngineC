#include "Shader.h"
#include "engine/ResourceManager.h"
#include <glad/glad.h>
#include <vector>

using namespace Core;

Shader::Shader() {

}

Shader::~Shader()
{
}

void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource)
{
	// compile shaders
	GLuint vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	initSamples(SHADER_SAMPLE_COUNT);
}

Shader& Shader::use()
{
	glUseProgram(ID);
	return *this;
}

void Shader::initSamples(unsigned char sampleCount) {
	std::vector<GLint> texIDs(sampleCount);
	for (GLint i = 0; i < sampleCount; i++) {
		texIDs[i] = i;
	}
	setIntegerV("textures", sampleCount, texIDs.data(), true);
}

void Shader::setFloat(const GLchar* name, GLfloat value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform1f(glGetUniformLocation(ID, name), value);
}
void Shader::setInteger(const GLchar* name, GLint value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setIntegerV(const GLchar* name, GLsizei count, GLint* value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform1iv(glGetUniformLocation(ID, name), count, value);
}

void Shader::setVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform2f(glGetUniformLocation(ID, name), x, y);
}
void Shader::setVector2f(const GLchar* name, const Vector2& value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}
void Shader::setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
void Shader::setVector3f(const GLchar* name, const Vector3& value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}
void Shader::setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
void Shader::setVector4f(const GLchar* name, const Vector4& value, GLboolean useShader)
{
	if (useShader)
		use();
	glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const GLchar* name, const Matrix4& matrix, GLboolean useShader)
{
	if (useShader)
		use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, matrix.getDataPtr());
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void Shader::setSourcePaths(const char* vertexPath, const char* fragmentPath) {
	this->vertexPath = vertexPath;
	this->fragmentPath = fragmentPath;
}

void Shader::serialize(std::ostream& os) const {
	std::size_t vertexSize = vertexPath.size();
	os.write((char*)&vertexSize, sizeof(vertexSize));
	os.write(vertexPath.c_str(), vertexPath.size());

	std::size_t fragmentSize = fragmentPath.size();
	os.write((char*)&fragmentSize, sizeof(fragmentSize));
	os.write(fragmentPath.c_str(), fragmentPath.size());
}

void Shader::deserialize(std::istream& is) {
	std::size_t vertexSize = vertexPath.size();
	is.read((char*)&vertexSize, sizeof(vertexSize));
	vertexPath.resize(vertexSize);
	is.read(&vertexPath[0], vertexSize);

	std::size_t fragmentSize = fragmentPath.size();
	is.read((char*)&fragmentSize, sizeof(fragmentSize));
	fragmentPath.resize(fragmentSize);
	is.read(&fragmentPath[0], fragmentSize);

	Shader shader = ResourceManager::getInstance().loadShader(vertexPath.substr(0, vertexPath.find_last_of(".")));
	this->ID = shader.ID;

	//std::cout << "Vertex: " << vertexSource << std::endl;
	//std::cout << "Fragment: " << fragmentSource << std::endl;

	//compile(vertexSource.c_str(), fragmentSource.c_str());
}