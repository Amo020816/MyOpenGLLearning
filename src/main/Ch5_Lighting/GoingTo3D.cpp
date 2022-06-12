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

// light position
glm::vec3 light_pos = glm::vec3(1.2f, 1.0f, 2.0f);

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

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Test For Lighting", NULL, NULL);	// create a window
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

    // shader program for cubes
    ShaderProgram shaderprogram_for_cube;

    shaderprogram_for_cube.CompileAndLink("assets/shaders/ShaderFor3DWorld/vsFor3D.glsl",
                                 "assets/shaders/ShaderFor3DWorld/fsForCube.glsl");

    //  shader program for light cube
    ShaderProgram shaderprogram_for_light;

    shaderprogram_for_light.CompileAndLink( "assets/shaders/ShaderFor3DWorld/vsFor3D.glsl",
                                            "assets/shaders/ShaderFor3DWorld/fsForLightPoint.glsl");

    // --------------------------------------------------------------------------------------------
    // vertex input

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
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
    // Vertex Array Object and Vertex Buffer Object

    // Cubes
    GLuint vao_cube, vbo_cube;;
    glGenVertexArrays(1, &vao_cube);	// generate a VAO for rectangle
    glBindVertexArray(vao_cube);	// bind VAO
    glGenBuffers(1, &vbo_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // texture
    GLuint mytexture;
    glGenTextures(1, &mytexture);		// generate a texture
    glBindTexture(GL_TEXTURE_2D, mytexture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
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

    // Light
    GLuint vao_light;
    glGenVertexArrays(1, &vao_light);
    glBindVertexArray(vao_light);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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
        //
        // drawing

        // -----------------------------------------------
        shaderprogram_for_cube.Bind();
        glBindTexture(GL_TEXTURE_2D, mytexture);
        glBindVertexArray(vao_cube);

        // calculate view, projection,matrix
        glm::mat4 view = glm::lookAt(  camera.GetCameraPos()
                ,camera.GetCameraPos() + camera.GetCameraFront()
                ,camera.GetCameraUp());

        glm::mat4 projection;
        projection = glm::perspective( glm::radians( camera.GetFov()), 800.0f / 600.0f, 0.1f, 100.0f);

        shaderprogram_for_cube.UploadMat4("view", view);
        shaderprogram_for_cube.UploadMat4("projection", projection);
        shaderprogram_for_cube.UploadVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 model = glm::mat4(1.0f);  // model matrix
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shaderprogram_for_cube.UploadMat4("model", model);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        shaderprogram_for_light.Bind();
        shaderprogram_for_light.UploadMat4("view", view);
        shaderprogram_for_light.UploadMat4("projection", projection);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        shaderprogram_for_light.UploadMat4("model", model);
        glBindVertexArray(vao_light);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        shaderprogram_for_light.Unbind();

        // check and call events, swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &vbo_cube);
    glDeleteVertexArrays(1, &vao_light);
    glDeleteTextures(1, &mytexture);
    glDeleteVertexArrays(1, &vao_cube);

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