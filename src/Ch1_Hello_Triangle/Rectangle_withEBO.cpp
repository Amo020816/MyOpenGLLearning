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
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// set glfw context version to 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// activate glfw core mode

	GLFWwindow* RectangelWindow = glfwCreateWindow(800, 600, "Rectangle with EBO", NULL, NULL);	// create a window
	if (RectangelWindow == NULL)
	{
		std::cout << "Failed to create a glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(RectangelWindow);
	glfwSetFramebufferSizeCallback(RectangelWindow, framebuffer_size_callback);

	// --------------------------------------------------------------------------------------------
	// glad : load all GLFW function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
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
	GLuint VAO_rectangle;
	glGenVertexArrays(1, &VAO_rectangle);	// generate a VAO for rectangle

	glBindVertexArray(VAO_rectangle);	// bind VAO

	// --------------------------------------------------------------------------------------------
	// vertex input
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	// Vertex Buffer Object
	GLuint VBO_rectangle;
	glGenBuffers(1, &VBO_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element Buffer Object
	GLuint EBO_rectangle;
	glGenBuffers(1, &EBO_rectangle);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_rectangle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set the vertex attributes potinters
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// --------------------------------------------------------------------------------------------
	// Render Loop
	while (!glfwWindowShouldClose(RectangelWindow))
	{
		// input
		processInput(RectangelWindow);

		glClear(GL_COLOR_BUFFER_BIT);

		// drawing
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO_rectangle);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// check and call events, swap buffers
		glfwPollEvents();
		glfwSwapBuffers(RectangelWindow);
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