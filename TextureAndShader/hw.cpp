#define GLEW_STATIC
#include <GL/glew.h>

// glew.h要求在glfw3.h前面
// glew.h会有OpenGL的必要头文件
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <random>
// SOIL 读取任意格式图片
#include <SOIL.h>
// glm 变换库
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
// action表示 按下/释放
// mode表示是否要加入ctrl, alt等操作
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
	******************初始化设置********************
	***********************************************/
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// 主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// 副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// 核心模式,不允许调整窗口大小,不允许使用旧函数
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "hello world", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Fail to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 初始化glew, 管理OpenGL函数指针
	// 要在窗口创建之后再初始化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Fail to initialize GLEW" << endl;
		return -1;
	}

	// 这里不直接设置width和height的值, 让它在高DPI屏幕上也能正常工作.
	// OpenGL坐标范围为-1到1
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, key_callback);

	/***********************************************
	******************shader设置********************
	***********************************************/

	Shader outShader = Shader("shader.vs", "shader.frag");

	GLfloat tri[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	
	/***********************************************
	*****************VAO,VBO设置********************
	***********************************************/

	GLfloat vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	// 生成
	GLuint VAO, VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	// 绑定
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// 添加数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 绑定VAO到上下文
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);	
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// TexCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// 解除绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/***********************************************
	*******************纹理1设置********************
	***********************************************/

	// 生成纹理对象
	GLuint texture1;
	glGenTextures(1, &texture1);
	// 绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 设置纹理环绕方式								// 重复纹理图像
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置过滤										// 线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// channel 渠道设置为0
	// SOIL_LOAD_RGB 只关注图片的RGB值
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	// 载入图片, 生成纹理
	glTexImage2D(GL_TEXTURE_2D,					// 目标
					0,							// 多级渐远纹理的级别, 0为基本级别. 如果不为0, 手动设置级别
					GL_RGB,						// 储存纹理的格式, 图片只有RGB, 所以纹理也这么存
					width,						// 宽
					height,						// 高
					0,							// 总是为0(历史遗留问题)
					GL_RGB,						// 源图的格式 
					GL_UNSIGNED_BYTE,			// 源图的数据类型
					image);						// 图片

	// 自动生成所有需要的多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	// 释放
	SOIL_free_image_data(image);

	/***********************************************
	*******************纹理2设置********************
	***********************************************/
	GLuint texture2;
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture2);
	// 设置纹理环绕方式								// 重复纹理图像
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置过滤										// 线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("am.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D,		// 目标
		0,							// 多级渐远纹理的级别, 0为基本级别. 如果不为0, 手动设置级别
		GL_RGB,						// 储存纹理的格式, 图片只有RGB, 所以纹理也这么存
		width,						// 宽
		height,						// 高
		0,							// 总是为0(历史遗留问题)
		GL_RGB,						// 源图的格式 
		GL_UNSIGNED_BYTE,			// 源图的数据类型
		image);						// 图片

	glGenerateMipmap(GL_TEXTURE_2D);
	// 释放
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	/***********************************************
	*******************旋转变换*********************
	***********************************************/

	glm::vec4 vec = { 1.0f, 0.0f, 0.0f, 1.0f };
	// 返回单位矩阵的lambda
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
		// 函数检查触发事件(键盘, 鼠标)
		glfwPollEvents();

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 画

		// 利用uniform配色
		//GLfloat time_val = glfwGetTime();
		//GLfloat green_val = (sin(time_val) / 2) + 0.5;
		//GLint vertexColorLocation = glGetUniformLocation(sp, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, green_val, 0.0f, 1.0f);
		//glUseProgram(sp);

		// 移动三角形
		/*if (off < 0.5f)
			off += 0.00005f;
		outShader.setFloat("offset", off);*/


		outShader.Use();

		// 纹理单元, 为了让着色器使用多个纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(outShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(outShader.Program, "ourTexture2"), 1);

		// 变换
		outShader.setFloat("percent", percent);

		

		trans = identity_matrix();
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		outShader.setMat4("transform", glm::value_ptr(trans));
		
		

		glBindVertexArray(VAO);
		//							 顶点个数
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = identity_matrix();
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		GLfloat scaleAmount = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

		outShader.setMat4("transform", glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// 交换颜色缓冲
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	// 释放资源
	glfwTerminate();

	return 0;
}