#define GLEW_STATIC
#include <GL/glew.h>

// glew.hҪ����glfw3.hǰ��
// glew.h����OpenGL�ı�Ҫͷ�ļ�
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

// action��ʾ����/�ͷ�
// mode��ʾ�Ƿ�Ҫ����ctrl, alt�Ȳ���
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		printf("press escape");
	}
}

GLuint shaderProgram[2];
void triangle_shader_init() {
	// ������ɫ��
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// vertexShaderSource ����ɫ��Դ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success1;
	GLchar infoLog1[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success1);

	if (!success1){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog1);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog1 << endl;
	}

	// Ƭ����ɫ��1
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
	// Ƭ����ɫ��2
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

	// �ϲ�������ɫ��

	shaderProgram[0] = glCreateProgram();
	shaderProgram[1] = glCreateProgram();
	// ��������ɫ��
	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	// ����
	glLinkProgram(shaderProgram[0]);
	glLinkProgram(shaderProgram[1]);
	// �жϺϲ��Ƿ�ɹ�
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
	// �ϲ���ɺ�ɾ��ԭ������ɫ������
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
	// ��ʼ��
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

	// ע��ص�����
	glfwSetKeyCallback(window, key_callback);

	/* 
	 * �����λ���ģ��
	 * ����shader�Լ�VBO,VAO
	 */
	
	triangle_shader_init();

	GLfloat vertices[] = {
		// ��һ��������
		0.0f, 0.5f, 0.0f,   // ����
		0.0f, -0.5f, 0.0f,  // ����
		0.5f, -0.5f, 0.0f,  // ���½�
		// �ڶ���������
		-0.5f, -0.5f, 0.0f, // ���½�
	};

	

	GLuint indices[] = {
		0, 1, 2,	// ��һ��������
		0, 1, 3		// �ڶ���������
	};

	GLuint VBO, VAO, EBO;
	// ���ɶ���
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	// ��VAO
	glBindVertexArray(VAO);

		// ���Ƶ�������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// ��������
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// ���ݴ�ʱGL_ARRAY_BBUFFER�е�VBO�Ѷ������
		// ��һ������ location = 0
		// �ڶ������� �������ԵĴ�С,Vec3,������3
		// ���������� ָ�����ݵ�����,GLSL��vec*���Ǹ��������
		// ���ĸ����� �����Ƿ��׼��. ��׼������0-1�ĸ�����
		// ��������� ����
		// ���������� �����ڻ�������ʼλ�õ�ƫ����
		// ���ö�������ָ��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		// ������ɫ����, ��������λ��ֵΪlocation = 0
		glEnableVertexAttribArray(0);

	// ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ���VAO 
	// EBO����Ҫ���
	glBindVertexArray(0);

	// ����OpenGL����ͼԪ�ķ���
	// ��һ������ Ӧ�õ����������ε�����ͱ���
	// �ڶ������� ����������
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	draw_2_triangle();

	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		// ������鴥���¼�(����, ���)
		glfwPollEvents();

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ��������
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
		

		// ������ɫ����
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(1, &VBO);

	// �ͷ���Դ
	glfwTerminate();

	return 0;
}