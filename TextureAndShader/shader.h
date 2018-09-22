#ifndef _SHADER_H_
#define _SHADER_H_
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>

// glm ±ä»»¿â
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
	void setFloat(const GLchar* name, GLfloat offset);
	void setMat4(const GLchar* name, const GLfloat* offset);
};



#endif