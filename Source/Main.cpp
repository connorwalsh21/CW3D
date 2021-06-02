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
#include <filesystem.h>

void deltaTime();
void viewportCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void getInput(GLFWwindow* window);
std::string getCurrentDirectory();
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(std::vector<std::string> faces);
void toggleFullscreen(GLFWwindow* window);

glm::vec3 lightPos(3.0f, -4.0f, 4.0f);

// Resolution
const unsigned int defaultResX = 1280;
const unsigned int defaultResY = 960;
unsigned int resX = 1280;
unsigned int resY = 960;
bool FULLSCREEN = false;

// Camera
bool firstMouse = true;
Camera camera(glm::vec3(3.55f, -3.8f, 12.70f));
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

    Shader myShader("resources/shaders/material.vs\0", "resources/shaders/material.fs\0");
    Shader skyboxShader("resources/shaders/skyboxvertex.vs\0", "resources/shaders/skyboxfrag.fs\0");
    Shader lightSourceShader("resources/shaders/lightsource.vs\0", "resources/shaders/lightsource.fs\0");

    // Vertice data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // World coordinate position
    const int size = 512; // This is the constant to change (Must be a perfect cube root)
    glm::vec3 cubePositions[size];
    int count = 0;
    for (int i = 0; i < cbrt(size); i++)
    {
        for (int j = -1; j > -1 - cbrt(size); j--)
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

    // Cube
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

    // Light source
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Skybox
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
    char texturesPathSpec[_MAX_PATH];
    strcpy(texturesPath, getCurrentDirectory().c_str());
    strcpy(texturesPathSpec, getCurrentDirectory().c_str());
    strcat(texturesPath, "\\resources\\textures\\container2.png");
    strcat(texturesPathSpec, "\\resources\\textures\\container2_specular.png");

    unsigned int diffuse = loadTexture(texturesPath);
    unsigned int specular = loadTexture(texturesPathSpec);

    std::vector<std::string> faces
    {
        "\\resources\\textures\\skybox\\spaceart.jpg",
        "\\resources\\textures\\skybox\\spaceart.jpg",
        "\\resources\\textures\\skybox\\spaceart.jpg",
        "\\resources\\textures\\skybox\\spaceart.jpg",
        "\\resources\\textures\\skybox\\spaceart.jpg",
        "\\resources\\textures\\skybox\\spaceart.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // Set textures to an ID
    myShader.use();
    myShader.setInt("material.diffuse", 0);
    myShader.setInt("material.specular", 1);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    
    // Main game loop
    while (!glfwWindowShouldClose(window))
    {
        deltaTime();

        // Handles input
        getInput(window);

        // Clear view
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Binds textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular);

        // Shader
        myShader.use();
        myShader.setVec3("light.position", lightPos);
        myShader.setVec3("viewPos", camera.Position);
        myShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        myShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        myShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("material.shininess", 64.0f);

        // Passes projection matrix to shader object
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)resX / (float)resY, 0.1f, 100.0f);
        myShader.setMat4("projection", projection);

        // Camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        myShader.setMat4("view", view);

        // Render loop for objects
        glBindVertexArray(VAO); // Change this for lighting VAO
        for (unsigned int i = 0; i < size; i++)
        {
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            myShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Configure light source shader
        lightSourceShader.use();
        lightSourceShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightSourceShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("view", view);

        // Render light source
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightSourceShader.setMat4("model", lightModel);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render loop for skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deletes vertex buffer object and vertex array object from GPU VRAM
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);
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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        toggleFullscreen(window);
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
    float yoffset = lastY - ypos;

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

unsigned int loadTexture(char const* path)
{
    unsigned int id;
    glGenTextures(1, &id);
    int x, y, numComponents;
    unsigned char* data = stbi_load(path, &x, &y, &numComponents, 0);
    if (data)
    {
        GLenum format;
        if (numComponents == 1)
        {
            format = GL_RED;
        }
        else if (numComponents == 3)
        {
            format = GL_RGB;
        }
        else if (numComponents == 4)
        {
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Cannot find texture file at: " << path << std::endl;
        stbi_image_free(data);
    }
    return id;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    stbi_set_flip_vertically_on_load(false);
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    int x, y, numChannel;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        char skyboxPath[_MAX_PATH];
        strcpy(skyboxPath, getCurrentDirectory().c_str());
        strcat(skyboxPath, faces[i].c_str());
        unsigned char* data = stbi_load(skyboxPath, &x, &y, &numChannel, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Could not load cubemap at: " << skyboxPath << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_set_flip_vertically_on_load(true);
    return id;
}

void toggleFullscreen(GLFWwindow* window)
{
    GLFWvidmode* mode;
    mode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!FULLSCREEN)
    {
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, resX = mode->width, resY = mode->height, mode->refreshRate);
        FULLSCREEN = !FULLSCREEN;
    }
    else
    {
        glfwSetWindowMonitor(window, nullptr, 100, 100, resX = defaultResX, resY = defaultResY, mode->refreshRate);
        FULLSCREEN = !FULLSCREEN;
    }
}