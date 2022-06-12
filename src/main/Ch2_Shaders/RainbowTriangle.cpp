#include <include/Core.h>
#include <include/Shader/Shader.h>
#include <include/Shader/ShaderProgram.h>



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
	std::array<Vertex, 3> vertices = {
					// colors	    					// positions
		Vertex{glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(  0.5f, -0.5f,  0.0f)},   // bottom right
		Vertex{glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f), glm::vec3( -0.5f, -0.5f,  0.0f)},   // bottom left
		Vertex{glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(  0.0f,  0.5f,  0.0f)}    // top 
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

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);





	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

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
		glDrawArrays(GL_TRIANGLES, 0, 3);
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