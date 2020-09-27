#ifndef SHADER_H
#define SHADER_H

#define SHADER_SAMPLE_COUNT 32

#include "maths/Vector2.h"
#include "maths/Vector3.h"
#include "maths/Vector4.h"
#include "maths/Matrix4.h"

#include "utils/Serializable.h"

#include <string>
#include <iostream>

typedef unsigned int	GLuint;
typedef char			GLchar;
typedef float			GLfloat;
typedef int				GLsizei;
typedef unsigned char	GLboolean;
typedef int				GLint;

namespace Core {
	class Shader : public Serializable {
	public:
		// the program ID
		GLuint ID = 0;
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
		void setVector2f(const GLchar* name, const Vector2 &value, GLboolean useShader = false);
		void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
		void setVector3f(const GLchar* name, const Vector3 &value, GLboolean useShader = false);
		void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
		void setVector4f(const GLchar* name, const Vector4 &value, GLboolean useShader = false);
		void setMatrix4(const GLchar* name, const Matrix4 &matrix, GLboolean useShader = false);

		bool operator==(const Shader& other) {
			return ID == other.ID;
		}

		void setSourcePaths(const char* vertexPath, const char* fragmentPath);

		void serialize(std::ostream& os) const override;
		void deserialize(std::istream& is) override;
	private:
		void initSamples(unsigned char sampleCount);
		void checkCompileErrors(unsigned int shader, std::string type);
	private:
		std::string vertexPath;
		std::string fragmentPath;
	};
}

#endif