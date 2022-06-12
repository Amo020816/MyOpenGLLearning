#include <Core.h>
#include <Shader/Shader.h>
#include <Shader/ShaderProgram.h>



void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

struct Vertex {
	glm::vec4 color;
	glm::vec3 position;
};


int main()
{
	// --------------------------------------------------------------------------------------------
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);					// set glfw context version to 4.6
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
	// Compile shaders and link shader program

	ShaderProgram shader;
	shader.CompileAndLink("assets/shaders/shaderCh2/basic.vs", "assets/shaders/shaderCh2/basic.fs");


	// --------------------------------------------------------------------------------------------
	//  Vertex Array Object
	GLuint VAO_rainbow;
	glGenVertexArrays(1, &VAO_rainbow);	// generate a VAO for rectangle

	glBindVertexArray(VAO_rainbow);	// bind VAO

	// --------------------------------------------------------------------------------------------
	// vertex input
	std::array<Vertex, 10> vertices = {
		   // colors	    				  // positions
	Vertex{{  0.4f, 0.521f, 0.960f, 1.0f},   {  -0.4f,  0.125f, 0.0f}},
	Vertex{{0.490f, 0.443f, 0.956f, 1.0f},   {-0.125f,  0.125f, 0.0f}},
	Vertex{{0.686f, 0.443f, 0.956f, 1.0f},   {   0.0f,    0.5f, 0.0f}},
	Vertex{{0.917f, 0.443f, 0.956f, 1.0f},   { 0.125f,  0.125f, 0.0f}},
	Vertex{{0.807f, 0.317f, 0.250f, 1.0f},   {   0.4f,  0.125f, 0.0f}},
	Vertex{{0.807f, 0.250f, 0.682f, 1.0f},   {  0.13f, -0.125f, 0.0f}},
	Vertex{{0.956f, 0.631f, 0.443f, 1.0f},   {  0.29f,   -0.6f, 0.0f}},
	Vertex{{0.956f, 0.843f, 0.443f, 1.0f},   {   0.0f,  -0.29f, 0.0f}},
	Vertex{{0.862f, 0.956f, 0.443f, 1.0f},   { -0.29f,   -0.6f, 0.0f}},
	Vertex{{0.584f, 0.956f, 0.443f, 1.0f},   { -0.13f, -0.125f, 0.0f}}
	};

	uint32 Elements[24] = {
		0, 1, 9,   1, 2, 3,
		3, 4, 5,   5, 6, 7,
		7, 8, 9,   9, 5, 7,
		9, 1, 3,   9, 3, 5
	};
	// Vertex Buffer Object
	GLuint VBO_rainbow;
	glGenBuffers(1, &VBO_rainbow);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_rainbow);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	// set the vertex attributes potinters
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint EBO_rainbow;
	glGenBuffers(1, &EBO_rainbow);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_rainbow);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements),	Elements, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// --------------------------------------------------------------------------------------------
	// Render Loop
	while (!glfwWindowShouldClose(RectangelWindow))
	{
		// input
		processInput(RectangelWindow);

		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// set PolygonMode to Line
		// drawing
		shader.Bind();
		glBindVertexArray(VAO_rainbow);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// check and call events, swap buffers
		glfwPollEvents();
		glfwSwapBuffers(RectangelWindow);
	}
	glDeleteBuffers(1, &VBO_rainbow);
	glDeleteBuffers(1, &EBO_rainbow);
	glDeleteVertexArrays(1, &VAO_rainbow);
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