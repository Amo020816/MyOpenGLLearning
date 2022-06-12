#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


std::string_view vertexShaderSource = R"(#version 330 core
		layout (location = 0) in vec3 aPos;
		void main()
		{
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		})";
// define the vertex shader source code in string_view in GLSL

std::string_view fragmentShaderSource = R"(#version 330 core
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		})";
// define the fragment shader source code in string_view in GLSL

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

int main()
{
	// --------------------------------------------------------------------------------------------
	// glfw initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// set context version to 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// core mode

	GLFWwindow* StarWindow = glfwCreateWindow(800, 600, "Star with EBO", NULL, NULL);
	if (StarWindow == NULL)
	{
		std::cout << "Failed to create a GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(StarWindow);		// make context current
	glfwSetFramebufferSizeCallback(StarWindow, framebuffer_size_callback);	


	// --------------------------------------------------------------------------------------------
	// glad : load all GLFW function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	// --------------------------------------------------------------------------------------------
	// build and compile shaders
	// vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSourceCstr = vertexShaderSource.data();
	
	glShaderSource(vertexShader, 1, &vertexShaderSourceCstr, NULL);
	glCompileShader(vertexShader);

	int _isSuccess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &_isSuccess);
	if (!_isSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShdaerSourceCstr = fragmentShaderSource.data();

	glShaderSource(fragmentShader, 1, &fragmentShdaerSourceCstr, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &_isSuccess);
	if (!_isSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// --------------------------------------------------------------------------------------------
	// Shader program
	GLuint shaderProgram_orange = glCreateProgram();	// create a shader program for orange

	glAttachShader(shaderProgram_orange, vertexShader);	
	glAttachShader(shaderProgram_orange, fragmentShader); // attach shader
	glLinkProgram(shaderProgram_orange);

	glGetProgramiv(shaderProgram_orange, GL_LINK_STATUS, &_isSuccess);
	if (!_isSuccess)
	{
		glGetProgramInfoLog(shaderProgram_orange, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADERPROGRAM_ORANGE::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// --------------------------------------------------------------------------------------------
	// vertex input
	float vertex_star[] = {
		 0.00f,  1.00f, 0.00f,		// 0
		 0.30f,  0.50f, 0.00f,		// 1
		 0.75f,  0.50f, 0.00f,		// 2
		 0.50f,  0.00f, 0.00f,		// 3
		 0.75f, -0.50f, 0.00f,		// 4
		 0.00f, -0.10f, 0.00f,		// 5
		-0.75f, -0.50f, 0.00f,		// 6
		-0.50f,  0.00f, 0.00f,		// 7
		-0.75f,  0.50f, 0.00f,		// 8
		-0.30f,  0.50f, 0.00f,		// 9
	};

	unsigned int indices_star[] = {
		0, 9, 1,
		1, 2, 3,
		1, 3, 5,
		3, 4, 5,
		1, 5, 9,
		5, 6, 7,
		5, 7, 9,
		7, 8, 9
	};

	// Vertex Array Object
	GLuint VAO_star;
	glGenVertexArrays(1, &VAO_star);
	glBindVertexArray(VAO_star);

	// Vertex Buffer Object
	GLuint VBO_star;
	glGenBuffers(1, &VBO_star);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_star);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_star), vertex_star, GL_STATIC_DRAW);

	// Element Buffer Object
	GLuint EBO_star;
	glGenBuffers(1, &EBO_star);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_star);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_star), indices_star, GL_STATIC_DRAW);

	// vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// --------------------------------------------------------------------------------------------
	// Render Loop
	while (!glfwWindowShouldClose(StarWindow))
	{
		// input
		processInput(StarWindow);

		glUseProgram(shaderProgram_orange);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO_star);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

		// check and call events, swap buffer
		glfwPollEvents();
		glfwSwapBuffers(StarWindow);
	}

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}