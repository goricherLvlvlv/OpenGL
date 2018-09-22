#define GLEW_STATIC
#include <GL/glew.h>

// glew.hҪ����glfw3.hǰ��
// glew.h����OpenGL�ı�Ҫͷ�ļ�
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <random>
// SOIL ��ȡ�����ʽͼƬ
#include <SOIL.h>
// glm �任��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
using std::cout;
using std::endl;
using std::cin;

const GLuint WIDTH = 800, HEIGHT = 600;

GLuint triangle_shader_init() {
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 color;\n"
		"out vec3 vcolor;\n"
		"void main() {\n"
		"gl_Position = vec4(position, 1.0);\n"
		"vcolor = color;}\0";

	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"in vec3 vcolor;\n"
		"out vec4 color;\n"
		"void main() {\n"
		"color = vec4(vcolor, 1.0f);}\0";

	

	GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(shaderVertex, 1, &vertexShaderSource, NULL);
	glShaderSource(shaderFragment, 1, &fragmentShaderSource, NULL);

	glCompileShader(shaderVertex);
	glCompileShader(shaderFragment);


	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaderFragment);
	glAttachShader(shaderProgram, shaderVertex);
	glLinkProgram(shaderProgram);
	glDeleteShader(shaderFragment);
	glDeleteShader(shaderVertex);

	

	return shaderProgram;
}

GLfloat percent = 0.0f;
// action��ʾ ����/�ͷ�
// mode��ʾ�Ƿ�Ҫ����ctrl, alt�Ȳ���
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		printf("press escape");
	}
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
		percent += 0.02f;
		printf("press up: percent = %lf\n", percent);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
		percent -= 0.02f;
		printf("press down: percent = %lf\n", percent);
	}
}

int main() {
	/***********************************************
	******************��ʼ������********************
	***********************************************/
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// ���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// ���汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// ����ģʽ,������������ڴ�С,������ʹ�þɺ���
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "hello world", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Fail to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// ��ʼ��glew, ����OpenGL����ָ��
	// Ҫ�ڴ��ڴ���֮���ٳ�ʼ��
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Fail to initialize GLEW" << endl;
		return -1;
	}

	// ���ﲻֱ������width��height��ֵ, �����ڸ�DPI��Ļ��Ҳ����������.
	// OpenGL���귶ΧΪ-1��1
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, key_callback);

	/***********************************************
	******************shader����********************
	***********************************************/

	Shader outShader = Shader("shader.vs", "shader.frag");

	GLfloat tri[] = {
		// λ��              // ��ɫ
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};
	
	/***********************************************
	*****************VAO,VBO����********************
	***********************************************/

	GLfloat vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	// ����
	GLuint VAO, VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	// ��
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// �������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// ��VAO��������
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);	
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// TexCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// �����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/***********************************************
	*******************����1����********************
	***********************************************/

	// �����������
	GLuint texture1;
	glGenTextures(1, &texture1);
	// ������
	glBindTexture(GL_TEXTURE_2D, texture1);

	// ���������Ʒ�ʽ								// �ظ�����ͼ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ���ù���										// ���Թ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// channel ��������Ϊ0
	// SOIL_LOAD_RGB ֻ��עͼƬ��RGBֵ
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	// ����ͼƬ, ��������
	glTexImage2D(GL_TEXTURE_2D,					// Ŀ��
					0,							// �༶��Զ����ļ���, 0Ϊ��������. �����Ϊ0, �ֶ����ü���
					GL_RGB,						// ��������ĸ�ʽ, ͼƬֻ��RGB, ��������Ҳ��ô��
					width,						// ��
					height,						// ��
					0,							// ����Ϊ0(��ʷ��������)
					GL_RGB,						// Դͼ�ĸ�ʽ 
					GL_UNSIGNED_BYTE,			// Դͼ����������
					image);						// ͼƬ

	// �Զ�����������Ҫ�Ķ༶��Զ����
	glGenerateMipmap(GL_TEXTURE_2D);
	// �ͷ�
	SOIL_free_image_data(image);

	/***********************************************
	*******************����2����********************
	***********************************************/
	GLuint texture2;
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture2);
	// ���������Ʒ�ʽ								// �ظ�����ͼ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ���ù���										// ���Թ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("am.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D,		// Ŀ��
		0,							// �༶��Զ����ļ���, 0Ϊ��������. �����Ϊ0, �ֶ����ü���
		GL_RGB,						// ��������ĸ�ʽ, ͼƬֻ��RGB, ��������Ҳ��ô��
		width,						// ��
		height,						// ��
		0,							// ����Ϊ0(��ʷ��������)
		GL_RGB,						// Դͼ�ĸ�ʽ 
		GL_UNSIGNED_BYTE,			// Դͼ����������
		image);						// ͼƬ

	glGenerateMipmap(GL_TEXTURE_2D);
	// �ͷ�
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	/***********************************************
	*******************��ת�任*********************
	***********************************************/

	glm::vec4 vec = { 1.0f, 0.0f, 0.0f, 1.0f };
	// ���ص�λ�����lambda
	auto identity_matrix = []() {return glm::mat4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	}; };

	auto print_matrix = [](glm::mat4 trans) {
		printf("%.4lf\t\t%.4lf\t\t%.4lf\t\t%.4lf\n", trans[0].x, trans[0].y, trans[0].z, trans[0].w);
		printf("%.4lf\t\t%.4lf\t\t%.4lf\t\t%.4lf\n", trans[1].x, trans[1].y, trans[1].z, trans[1].w);
		printf("%.4lf\t\t%.4lf\t\t%.4lf\t\t%.4lf\n", trans[2].x, trans[2].y, trans[2].z, trans[2].w);
		printf("%.4lf\t\t%.4lf\t\t%.4lf\t\t%.4lf\n", trans[3].x, trans[3].y, trans[3].z, trans[3].w);
		
		cout << endl;
	};

	glm::mat4 trans = identity_matrix();
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


	GLfloat off = 0.0f;

	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		// ������鴥���¼�(����, ���)
		glfwPollEvents();

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ��

		// ����uniform��ɫ
		//GLfloat time_val = glfwGetTime();
		//GLfloat green_val = (sin(time_val) / 2) + 0.5;
		//GLint vertexColorLocation = glGetUniformLocation(sp, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, green_val, 0.0f, 1.0f);
		//glUseProgram(sp);

		// �ƶ�������
		/*if (off < 0.5f)
			off += 0.00005f;
		outShader.setFloat("offset", off);*/


		outShader.Use();

		// ����Ԫ, Ϊ������ɫ��ʹ�ö������
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(outShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(outShader.Program, "ourTexture2"), 1);

		// �任
		outShader.setFloat("percent", percent);

		

		trans = identity_matrix();
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		outShader.setMat4("transform", glm::value_ptr(trans));
		
		

		glBindVertexArray(VAO);
		//							 �������
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = identity_matrix();
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		GLfloat scaleAmount = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

		outShader.setMat4("transform", glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// ������ɫ����
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	// �ͷ���Դ
	glfwTerminate();

	return 0;
}