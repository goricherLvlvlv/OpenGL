#define GLEW_STATIC
#include <GL/glew.h>

// glew.h要求在glfw3.h前面
// glew.h会有OpenGL的必要头文件
#include <GLFW/glfw3.h>
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"void main() {\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);}\0";

const GLchar* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(0.0f, 1.0f, 1.0f, 1.0f);}\0";

// action表示按下/释放
// mode表示是否要加入ctrl, alt等操作
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		printf("press escape");
	}
}

GLuint shaderProgram[2];
void triangle_shader_init() {
	// 顶点着色器
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// vertexShaderSource 是着色器源码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success1;
	GLchar infoLog1[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success1);

	if (!success1){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog1);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog1 << endl;
	}

	// 片段着色器1
	GLuint fragmentShader[2];
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader[0], 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader[0]);

	GLint success2;
	GLchar infoLog2[512];
	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success2);

	if (!success2) {
		glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLog2);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog2 << endl;
	}
	// 片段着色器2
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader[1], 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader[1]);

	GLint success3;
	GLchar infoLog3[512];
	glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success3);

	if (!success3) {
		glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLog3);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog3 << endl;
	}

	// 合并两个着色器

	shaderProgram[0] = glCreateProgram();
	shaderProgram[1] = glCreateProgram();
	// 加入子着色器
	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	// 链接
	glLinkProgram(shaderProgram[0]);
	glLinkProgram(shaderProgram[1]);
	// 判断合并是否成功
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
	}

	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
	}
	// 合并完成后删除原来的着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);

}
GLuint VBOs[2], VAOs[2];
void draw_2_triangle() {
	GLfloat firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // Left 
		-0.0f, -0.5f, 0.0f,  // Right
		-0.45f, 0.5f, 0.0f,  // Top 
	};
	GLfloat secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // Left
		0.9f, -0.5f, 0.0f,  // Right
		0.45f, 0.5f, 0.0f   // Top 
	};

	
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}

int main() {
	// 初始化
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

	// 注册回调函数
	glfwSetKeyCallback(window, key_callback);

	/* 
	 * 三角形绘制模块
	 * 创建shader以及VBO,VAO
	 */
	
	triangle_shader_init();

	GLfloat vertices[] = {
		// 第一个三角形
		0.0f, 0.5f, 0.0f,   // 中上
		0.0f, -0.5f, 0.0f,  // 中下
		0.5f, -0.5f, 0.0f,  // 右下角
		// 第二个三角形
		-0.5f, -0.5f, 0.0f, // 左下角
	};

	

	GLuint indices[] = {
		0, 1, 2,	// 第一个三角形
		0, 1, 3		// 第二个三角形
	};

	GLuint VBO, VAO, EBO;
	// 生成对象
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	// 绑定VAO
	glBindVertexArray(VAO);

		// 复制到缓冲区
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 建立索引
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 根据此时GL_ARRAY_BBUFFER中的VBO把顶点加入
		// 第一个参数 location = 0
		// 第二个参数 顶点属性的大小,Vec3,所以是3
		// 第三个参数 指定数据的类型,GLSL中vec*都是浮点数组成
		// 第四个参数 数据是否标准化. 标准化就是0-1的浮点数
		// 第五个参数 步长
		// 第六个参数 数据在缓冲中起始位置的偏移量
		// 设置顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		// 顶点着色器中, 顶点属性位置值为location = 0
		glEnableVertexAttribArray(0);

	// 解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑VAO 
	// EBO不需要解绑
	glBindVertexArray(0);

	// 配置OpenGL绘制图元的方法
	// 第一个参数 应用到所有三角形的正面和背面
	// 第二个参数 用线来绘制
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	draw_2_triangle();

	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		// 函数检查触发事件(键盘, 鼠标)
		glfwPollEvents();

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 画三角形
		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glUseProgram(shaderProgram[0]);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		

		// 交换颜色缓冲
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(1, &VBO);

	// 释放资源
	glfwTerminate();

	return 0;
}