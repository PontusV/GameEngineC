#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

typedef unsigned int	GLuint;
typedef char			GLchar;
typedef float			GLfloat;
typedef int				GLsizei;
typedef unsigned char	GLboolean;
typedef int				GLint;

namespace Core {
	class Shader
	{
	public:
		// the program ID
		GLuint ID;
		Shader();
		~Shader();
		// use/activate the shader
		Shader& use();
		// compile program
		void compile(const GLchar* vertexSource, const GLchar* fragmentSource);
		// utility uniform functions
		void setFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
		void setInteger(const GLchar* name, GLint value, GLboolean useShader = false);
		void setIntegerV(const GLchar* name, GLsizei count, GLint* value, GLboolean useShader = false);
		void setVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
		void setVector2f(const GLchar* name, const glm::vec2 &value, GLboolean useShader = false);
		void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
		void setVector3f(const GLchar* name, const glm::vec3 &value, GLboolean useShader = false);
		void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
		void setVector4f(const GLchar* name, const glm::vec4 &value, GLboolean useShader = false);
		void setMatrix4(const GLchar* name, const glm::mat4 &matrix, GLboolean useShader = false);

		bool operator==(const Shader& other) {
			return ID == other.ID;
		}
	private:
		void checkCompileErrors(unsigned int shader, std::string type);
	};
}

#endif