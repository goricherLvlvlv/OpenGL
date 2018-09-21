#ifndef _SHADER_H_
#define _SHADER_H_
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>

class Shader {
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
	void setFloat(const GLchar* name, GLfloat offset);
};


#endif