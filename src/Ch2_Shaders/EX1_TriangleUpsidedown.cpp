#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


std::string_view vertexShaderSource = R"(#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;

		out vec3 ourColor;		

		void main()
		{
		   gl_Position = vec4(aPos, 1.0);
		   ourColor = aColor;
		})";
// define the vertex shader source code in string_view in GLSL

std::string_view fragmentShaderSource = R"(#version 330 core
		out vec4 FragColor;
		in vec3 ourColor;
		void main()
		{
			FragColor = vec4(ourColor, 1.0);
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

	GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle Upside Down", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// --------------------------------------------------------------------------------------------
	// glad : load all GLFW functions pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD\n";
		return -1;
	}

	// --------------------------------------------------------------------------------------------
	// Compile shaders
	// vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);		// create vertex shader
	const GLchar* vertexShaderSourceCstr = vertexShaderSource.data();
	glShaderSource(vertexShader, 1, &vertexShaderSourceCstr, NULL);		// link vertex shader source
	glCompileShader(vertexShader);

	int _isSuccess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &_isSuccess);
	if (!_isSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;		// Vertex shader compilation error infoLog
	}

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCstr = fragmentShaderSource.data();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCstr, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &_isSuccess);
	if (!_isSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;	// Fragment shader compilation error infoLog
	}

	// --------------------------------------------------------------------------------------------
	// Shader Program
	GLuint shaderProgram = glCreateProgram();			// Create shader Program

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);		// attach shaders

	glLinkProgram(shaderProgram);						//  link shaders

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &_isSuccess);
	if (!_isSuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);				// once linked, delete the shaders


	// --------------------------------------------------------------------------------------------
	//  Vertex Array Object
	GLuint VAO_rainbow;
	glGenVertexArrays(1, &VAO_rainbow);	// generate a VAO for rectangle

	glBindVertexArray(VAO_rainbow);	// bind VAO

	// --------------------------------------------------------------------------------------------
	// vertex input
	float vertices[] = {
		// positions         // colors
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // top right
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // top left
		 0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // bottom
	};
	// Vertex Buffer Object
	GLuint VBO_rainbow;
	glGenBuffers(1, &VBO_rainbow);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_rainbow);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set the vertex attributes potinters
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// --------------------------------------------------------------------------------------------
	// Rendor Loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// set PolygonMode to Line
		// drawing
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO_rainbow);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// check and call events, swap buffer
		glfwPollEvents();
		glfwSwapBuffers(window);

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