#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <shader.h>
#include <iostream>
#include <Windows.h>
#include <string>

void deltaTime();
void viewportCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void getInput(GLFWwindow* window);
std::string getCurrentDirectory();

// Resolution
const unsigned int resX = 1280;
const unsigned int resY = 960;

bool firstMouse = true;
Camera camera(glm::vec3(3.55f, -3.8f, 12.70f)); // (glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = resX / 2.0;
float lastY = resY / 2.0;

// Delta time value
float dt = 0.0f;
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(resX, resY, "CW3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, viewportCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Loading GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader myShader;

    // Vertice data
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

    // World coordinate position
    const int size = 512; // This is the constant to change (Must be a perfect cube root)
    glm::vec3 cubePositions[size];
    int count = 0;
    for (int i = 0; i < cbrt(size); i++)
    {
        for (int j = -1; j > -1-cbrt(size); j--)
        {
            for (int k = 0; k < cbrt(size); k++)
            {
                if (i == 0 || i == cbrt(size) - 1 || j == -1 || j == -cbrt(size) || k == 0 || k == cbrt(size))
                {
                    cubePositions[count] = glm::vec3(i, j, k);
                }
                count++;
            }
        }
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Mipmaps
    int width, height, nrChannels;

    // Flips textures on Y-axis
    stbi_set_flip_vertically_on_load(true);

    // Textures code
    char texturesPath[_MAX_PATH];
    strcpy(texturesPath, getCurrentDirectory().c_str());
    strcat(texturesPath, "\\resources\\textures\\demon.jpg");
    std::cout << texturesPath << std::endl;

    unsigned char* data = stbi_load(texturesPath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // This must be a PNG for the alpha channel
    data = stbi_load("C:\\Users\\Connor\\source\\repos\\CW3D\\resources\\textures\\noimage", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture - Although for now, this is supposed to happen!" << std::endl;
    }
    stbi_image_free(data);

    // Set textures to an ID
    myShader.use();
    myShader.setInt("texture1", 0);
    myShader.setInt("texture2", 1);

    // Main game loop
    while (!glfwWindowShouldClose(window))
    {
        deltaTime();

        // Handles input
        getInput(window);

        // Clear view
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Binds textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        myShader.use();

        // Passes projection matrix to shader object
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)resX / (float)resY, 0.1f, 100.0f);
        myShader.setMat4("projection", projection);

        // Camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        myShader.setMat4("view", view);

        // Render loop for objects
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < size; i++)
        {
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            myShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();

        // For meme purposes
        //std::cout << camera.Position.x << camera.Position.y << camera.Position.z << std::endl;
    }

    // Deletes vertex buffer object and vertex array object from GPU VRAM
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void getInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.HandleKeyboard(FORWARD, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.HandleKeyboard(BACKWARD, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.HandleKeyboard(LEFT, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.HandleKeyboard(RIGHT, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.HandleKeyboard(UP, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera.HandleKeyboard(DOWN, dt);
    }
}

void deltaTime()
{
    float currentFrame = glfwGetTime();
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void viewportCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.HandleMouse(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.HandleScroll(yoffset);
}

std::string getCurrentDirectory()
{
    char b[MAX_PATH];
    GetModuleFileNameA(NULL, b, MAX_PATH);
    std::string::size_type loc = std::string(b).find_last_of("\\/");
    return std::string(b).substr(0, loc);
}
