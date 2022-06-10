#include <Core.h>
#include <Shader/Shader.h>
#include <Shader/ShaderProgram.h>
// stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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
    // load and compile shaders, link shader program
    ShaderProgram shaderProgram;

    shaderProgram.CompileAndLink("assets/shaders/ShadersForTexture/vsForTexture.vs",
                                 "assets/shaders/ShadersForTexture/fsForTexture.fs");



    // --------------------------------------------------------------------------------------------
	// vertex input
	float vertices[] = {
		// positions         // colors          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left	
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// --------------------------------------------------------------------------------------------
	//  Vertex Array Object
	GLuint VAO_rectangle;
	glGenVertexArrays(1, &VAO_rectangle);	// generate a VAO for rectangle

	glBindVertexArray(VAO_rectangle);	// bind VAO

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// --------------------------------------------------------------------------------------------
	// texture
	GLuint mytexture;
	glGenTextures(1, &mytexture);		// generate a texture
	glBindTexture(GL_TEXTURE_2D, mytexture);

	// set the texture wrapping/filitering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/res/haha.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);



	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// --------------------------------------------------------------------------------------------
	// Render Loop
	while (!glfwWindowShouldClose(RectangelWindow))
	{
		// input
		processInput(RectangelWindow);

		glClear(GL_COLOR_BUFFER_BIT);

		// drawing
        shaderProgram.Bind();

        glBindTexture(GL_TEXTURE_2D, mytexture);
        glBindVertexArray(VAO_rectangle);
        // remember that first scale then rotate at last translate
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.5,0.5,0.5));
        trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
        shaderProgram.UploadMat4("transform", trans);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        trans = glm::scale(trans, glm::vec3(abs(sin((float)glfwGetTime()))));

        shaderProgram.UploadMat4("transform", trans);
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