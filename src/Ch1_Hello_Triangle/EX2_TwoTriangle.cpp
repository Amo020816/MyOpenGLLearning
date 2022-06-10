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

std::string_view fragmentShaderSource_Orange = R"(#version 330 core
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		})";
// define the fragment shader source code in string_view in GLSL


//0.541, 0.169, 0.886
std::string_view fragmentShaderSource_Voilet = R"(#version 330 core
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(0.541f, 0.169f, 0.886f, 1.0f);
		})";

static bool _isChange = 0;
void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

int main()
{
	// glfw: initialize and configure
	// ---------------------------------------------------------------------------------------------------------------------------------
	glfwInit();	// initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);						// set GLFW version to 3.3
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
	}	// Load GLAD to manage function pointers for OpenGL, because pointers differ from OpenGL ve


	// build and compile shader program
	// -----------------------------------------------------------------------------------------------------------------------------------
	// vertex shader

	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertexShadersourceCstr = vertexShaderSource.data();
	glShaderSource(vertexshader, 1, &vertexShadersourceCstr, NULL);  // bind vertex shader source
	glCompileShader(vertexshader);						 // compile vertex shader

	int success;
	char infoLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);		// get compile status of vertexshader
	if (!success) {
		glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		// if compiling failed, get information of failure
	}

	// fragment shader Orange
	GLuint fragmentShader_Orange = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShadersource_Orange_Cstr = fragmentShaderSource_Orange.data();
	glShaderSource(fragmentShader_Orange, 1, &fragmentShadersource_Orange_Cstr, NULL);
	glCompileShader(fragmentShader_Orange);

	glGetShaderiv(fragmentShader_Orange, GL_COMPILE_STATUS, &success);		// get compile status of fragment shader
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_Orange, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT_ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader Voilet
	GLuint fragmentShader_Voilet = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource_Voilet_Cstr = fragmentShaderSource_Voilet.data();
	glShaderSource(fragmentShader_Voilet, 1, &fragmentShaderSource_Voilet_Cstr, NULL);
	glCompileShader(fragmentShader_Voilet);

	glGetShaderiv(fragmentShader_Voilet, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_Voilet, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER:FRAGMENT_VIOLET::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	// --------------------------------------------------------------------------------------------------------------------
	GLuint shaderProgram_Orange = 0;
	shaderProgram_Orange = glCreateProgram(); // create a program

	glAttachShader(shaderProgram_Orange, vertexshader);
	glAttachShader(shaderProgram_Orange, fragmentShader_Orange);	// attach the shader previously compiled

	glLinkProgram(shaderProgram_Orange);	// link the program

	glGetProgramiv(shaderProgram_Orange, GL_LINK_STATUS, &success);		// get link status of shader program
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram_Orange, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER_ORANGE::LINK_FAILED\n" << infoLog << std::endl;
		// if link failed, get the info
	}

	// shaderProgram Violet
	GLuint shaderProgram_Violet = glCreateProgram();
	
	glAttachShader(shaderProgram_Violet, vertexshader);
	glAttachShader(shaderProgram_Violet, fragmentShader_Voilet);
	glLinkProgram(shaderProgram_Violet);

	glGetProgramiv(shaderProgram_Violet, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram_Violet, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER_VIOLET::LINK_FAILED\n" << infoLog << std::endl;
	}

	//glUseProgram(shaderProgram);	// activate the shader program

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentShader_Orange);		// once linked, delete the shader
	glDeleteShader(fragmentShader_Voilet);

	// Vertex Array Object
	// ----------------------------------------------------------------------------------------------------------------------------------
	GLuint VAO_Orange;
	glGenVertexArrays(1, &VAO_Orange);

	glBindVertexArray(VAO_Orange);

	// Vertex input
	// ----------------------------------------------------------------------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	GLuint VBO_Orange;
	glGenBuffers(1, &VBO_Orange);			// generate one buffer to refrence ID of VBO

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Orange);		// Bind VBO to GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy vertices data to GL_ARRAY_BUFFER i.e. VBO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);		// unbind GL_ARRAY_BUFFER

	// Linking Vertex Attributes
	// -----------------------------------------------------------------------------------------------------------------------------------
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// interpret the vertex data 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	
	// -----------------------------------------------------------------------------------------------------------------------------------
	// Violet version
	GLuint VAO_Violet;
	glGenVertexArrays(1, &VAO_Violet);
	glBindVertexArray(VAO_Violet);

	GLuint VBO_Violet;
	glGenBuffers(1, &VBO_Violet);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Violet);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// interpret the vertex data 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

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

		if (_isChange)
		{
			glUseProgram(shaderProgram_Orange);	// activate the shader program	
			glBindVertexArray(VAO_Orange);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}
		else
		{
			glUseProgram(shaderProgram_Violet);
			glBindVertexArray(VAO_Violet);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}
		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO_Orange);
	glDeleteVertexArrays(1, &VAO_Violet);
	glDeleteBuffers(1, &VBO_Orange);
	glDeleteBuffers(1, &VBO_Violet);
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

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
		_isChange = !_isChange;						// 他这个感应太灵敏了
}