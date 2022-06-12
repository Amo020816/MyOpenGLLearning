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
	// glfw: initialize and configure
	// ---------------------------------------------------------------------------------------------------------------------------------
	glfwInit();	// initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);						// set GLFW context version to 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// Tell GLFW we want to explicitly use the core-profile
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			For Mac OS X

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Create a window
	glfwMakeContextCurrent(window);		// Make Context Current to the window ?
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		// adjust the viewport to the window size

	// glad : load all OpenGL function pointers
	// -----------------------------------------------------------------------------------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}	// Load GLAD to manage function pointers for OpenGL, because pointers differ from OpenGL version


	// build and compile shader program
	// -----------------------------------------------------------------------------------------------------------------------------------
	// vertex shader

	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertexShadersourceCstr = vertexShaderSource.data();
	glShaderSource(vertexshader, 1, &vertexShadersourceCstr, NULL);  // bind vertex shader source
	glCompileShader(vertexshader);						 // compile vertex shader

	// fragment shader 
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShadersourceCstr = fragmentShaderSource.data();
	glShaderSource(fragmentShader, 1, &fragmentShadersourceCstr, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);		// get compile status of vertexshader
	if (!success) {
		glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		// if compiling failed, get information of failure
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);		// get compile status of fragment shader
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	// --------------------------------------------------------------------------------------------------------------------
	GLuint shaderProgram = 0;
	shaderProgram = glCreateProgram(); // create a program

	glAttachShader(shaderProgram, vertexshader);
	glAttachShader(shaderProgram, fragmentShader);	// attach the shader previously compiled

	glLinkProgram(shaderProgram);	// link the program

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);		// get link status of shader program
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		// if link failed, get the info
	}

	//glUseProgram(shaderProgram);	// activate the shader program

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentShader);		// once linked, delete the shader

	// Vertex Array Object
	// ----------------------------------------------------------------------------------------------------------------------------------
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// Vertex input
	// ----------------------------------------------------------------------------------------------------------------------------------
	float vertices[] = {
		-1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 1.0f,  0.0f, 0.0f
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);			// generate one buffer to refrence ID of VBO

	glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Bind VBO to GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy vertices data to GL_ARRAY_BUFFER i.e. VBO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);		// unbind GL_ARRAY_BUFFER

	// Linking Vertex Attributes
	// -----------------------------------------------------------------------------------------------------------------------------------
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// interpret the vertex data 
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);								// state-setting



	// Render Loop
	// -----------------------------------------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		// ...
		glClear(GL_COLOR_BUFFER_BIT);									// state-using

		glUseProgram(shaderProgram);	// activate the shader program	
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
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