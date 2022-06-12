#include <Core.h>
#include <Shader/ShaderProgram.h>
#include <Camera/Camera.h>
// stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// deltaTime
float deltaTime = 0.0f;     // Time between current frame and last frame
float lastFrame = 0.0f;     // Time of last frame

int window_width = 800;
int window_height = 600;

const glm::vec3 Camera::world_up = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera( (float)window_width, (float)window_height);

// some callback functions
void FrameBufferSizeCallBack(GLFWwindow*, int, int);
void MouseMovementCallBack(GLFWwindow*, double, double);
void MouseScrollCallBack(GLFWwindow*, double, double);

// process input from the user
void processInput(GLFWwindow*);


int main()
{
    // --------------------------------------------------------------------------------------------
    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// set glfw context version to 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// activate glfw core mode

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Rectangle with EBO", NULL, NULL);	// create a window
    if (window == nullptr)
    {
        std::cout << "Failed to create a glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);
    glfwSetCursorPosCallback(window, MouseMovementCallBack);
    glfwSetScrollCallback(window, MouseScrollCallBack);

    // --------------------------------------------------------------------------------------------
    // glad : load all GLFW function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }

    // --------------------------------------------------------------------------------------------
    // load and compile shaders, link shader program
    ShaderProgram shaderprogram_for_cube;

    shaderprogram_for_cube.CompileAndLink("assets/shaders/ShaderFor3D/vsFor3D.glsl",
                                 "assets/shaders/ShaderFor3D/fsFor3D.glsl");



    // --------------------------------------------------------------------------------------------
    // vertex input
    /*
    float vertices[] = {
            // positions                      // colors          // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    */

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    /*
    // Element Buffer Object
    GLuint EBO_rectangle;
    glGenBuffers(1, &EBO_rectangle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_rectangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */
    // set the vertex attributes potinters
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(1);

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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    // --------------------------------------------------------------------------------------------
    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // drawing
        shaderprogram_for_cube.Bind();

        glBindTexture(GL_TEXTURE_2D, mytexture);
        glBindVertexArray(VAO_rectangle);

        glm::mat4 view = glm::lookAt(  camera.GetCameraPos()
                ,camera.GetCameraPos() + camera.GetCameraFront()
                ,camera.GetCameraUp());

        glm::mat4 projection;
        projection = glm::perspective( glm::radians( camera.GetFov()), 800.0f / 600.0f, 0.1f, 100.0f);

        shaderprogram_for_cube.UploadMat4("view", view);
        shaderprogram_for_cube.UploadMat4("projection", projection);
        shaderprogram_for_cube.UploadVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));

        for (uint32 i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);  // model matrix
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderprogram_for_cube.UploadMat4("model", model);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }


        glBindVertexArray(0);

        // check and call events, swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO_rectangle);
    glDeleteTextures(1, &mytexture);
    glDeleteVertexArrays(1, &VAO_rectangle);

    glfwTerminate();
    return 0;
}

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseMovementCallBack(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
  camera.InsMouseMovementCallBack(window, x_pos_in, y_pos_in);
}


void MouseScrollCallBack(GLFWwindow* window, double x_in, double y_in)
{
  camera.InsMouseScrollCallBack(window, x_in, y_in);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

  // --------------------------------------------------------------------------------------------
  // process input for camera moving
    const float kCameraSpeed = 2.5f * deltaTime;         // 2.5 unit per second (actual displacement)

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.CameraMoveBy(FORWARD, kCameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.CameraMoveBy(BACKWARD, kCameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.CameraMoveBy(RIGHT, kCameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.CameraMoveBy(LEFT, kCameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      camera.CameraMoveBy(UPWARD, kCameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      camera.CameraMoveBy(DOWNWARD, kCameraSpeed);
}