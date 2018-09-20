#ifndef _DRAW_TRIANGLE_H_
#define _DRAW_TRIANGLE_H_
#define GLEW_STATIC
#include <GL/glew.h>

// glew.h要求在glfw3.h前面
// glew.h会有OpenGL的必要头文件
#include <GLFW/glfw3.h>
#include <iostream>
#include <initializer_list>
#include <vector>
using namespace std;
enum shaderType {
	vertex_shader,
	fragment_shader
};
class triangle {
public:
	static GLuint shader_init(const GLchar* shader_src, shaderType type) {
		GLuint shader;
		if (type == vertex_shader)
			shader = glCreateShader(GL_VERTEX_SHADER);
		else if (type == fragment_shader)
			shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(shader, 1, &shader_src, NULL);
		glCompileShader(shader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
		}

		return shader;
	}

	static GLuint shader_merge(initializer_list<GLuint> shaders) {
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();
		for (auto _s : shaders) {
			glAttachShader(shaderProgram, _s);
			glDeleteShader(_s);
		}
		glLinkProgram(shaderProgram);

		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		}

		return shaderProgram;
	}

private:


};


#endif